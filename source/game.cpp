#include "game.h"

#include <sys/dir.h>
#include <filesystem>
#include "menu/main/titlemenu.h"
#include "menu/world/leveltransitionmenu.h"
#include "menu/player/deadmenu.h"
#include "menu/player/wonmenu.h"
#include "nbt.h"

namespace fs = std::filesystem;

const std::string Game::NAME = "Minicraft DS";
const std::string Game::SAVE_FOLDER = "minicraft/saves/";
const std::string Game::SAVE_EXTENSION = ".dat";

Game::Game()
  : lightMask(screen)
{
  setMenu(std::make_unique<TitleMenu>());
}

void Game::tick()
{
  tickCount++;

  scanKeys();
  tappedKeys = keysDown();
  heldKeys = keysHeld();
  tappedOrRepeatKeys = keysDownRepeat();
  releasedKeys = keysUp();
  touchRead(&touchPos);

  bool blockTick = false;

  if (menu != NULL) {
    blockTick = menu->blocksGameTick;
    menu->tick(*this);
  }

  if (blockTick)
    return;

  if (player->removed) {
    playerDeadTime++;

    if (playerDeadTime > 60) {
      setMenu(std::make_unique<DeadMenu>(gameTime, player->score));
    }
  } else {
    if (!hasWon)
      gameTime++;

    if (pendingLevelChange != 0) {
      setMenu(std::make_unique<LevelTransitionMenu>(pendingLevelChange));
      pendingLevelChange = 0;
    }
  }

  if (wonTimer > 0) {
    if (--wonTimer == 0) {
      setMenu(std::make_unique<WonMenu>(gameTime, player->score));
    }
  }

  levels[currentLevel].tick(*this);
  Tile::tickCount++;
}

bool Game::isHeld(int key)
{
  return heldKeys & key;
}

bool Game::justTapped(int key)
{
  return tappedKeys & key;
}

bool Game::justTappedOrRepeat(int key)
{
  return tappedOrRepeatKeys & key;
}

bool Game::justReleased(int key)
{
  return releasedKeys & key;
}

int Game::touchX()
{
  return touchPos.px;
}

int Game::touchY()
{
  return touchPos.py;
}

void Game::render()
{
  if (player != NULL) {
    Level& level = levels[currentLevel];

    level.render(screen, lightMask, *player);
  }

  if (menu != NULL) {
    menu->render(screen, bottomScreen);
  }
}

void Game::setMenu(std::unique_ptr<Menu> menu)
{
  this->menu = std::move(menu);
}

void Game::enterMenu(std::unique_ptr<Menu> menu)
{
  menu->setParent(std::move(this->menu));

  this->menu = std::move(menu);
}

void Game::scheduleLevelChange(int dir)
{
  pendingLevelChange = dir;
}

void Game::changeLevel(int dir)
{
  levels[currentLevel].remove(player);
  currentLevel += dir;
  player->x = (player->x >> 4) * 16 + 8;
  player->y = (player->y >> 4) * 16 + 8;
  levels[currentLevel].add(player);
}

void Game::win()
{
  wonTimer = 60 * 3;
  hasWon = true;
}

void Game::resetGame()
{
  worldName = "";
  levels.clear();
  player = NULL;
  playerDeadTime = 0;
  pendingLevelChange = 0;
  wonTimer = 0;
  gameTime = 0;
  hasWon = false;
  Entity::resetIds();
}

std::string Game::savePath(const std::string& name)
{
  return Game::SAVE_FOLDER + name + Game::SAVE_EXTENSION;
}

void Game::resolveNameConflict() {
  int i = 2;
  auto originalName = worldName;

  while (fs::exists(savePath(worldName))) {
    worldName = originalName + " " + std::to_string(i);
    i++;
  }
}

static inline void ensureFolder(const std::string& path) {
  size_t lastSlashIndex = 0;

  while (true) {
    lastSlashIndex = path.find("/", lastSlashIndex + 1);

    if (lastSlashIndex == std::string::npos) {
      break;
    }

    auto substr = path.substr(0, lastSlashIndex);
    mkdir(substr.c_str(), 0777);
  }

  if (lastSlashIndex + 1 != path.size()) {
    mkdir(path.c_str(), 0777);
  }
}

void Game::save() {
  ensureFolder(Game::SAVE_FOLDER);

  auto fileName = savePath(worldName);
  auto s = std::ofstream(fileName.c_str(), std::ios::out | std::ios::binary);

  nbt::begin_named_compound(s, "");

  nbt::write_named_int(s, "GameTime", gameTime);
  nbt::begin_named_list(s, "Levels", nbt::Tag::COMPOUND, levels.size());

  for (auto& level : levels) {
    level.serialize(s);
    nbt::close_compound(s);
  }

  nbt::close_compound(s);
}

void Game::load(std::string worldName) {
  resetGame();

  this->worldName = worldName;

  auto fileName = savePath(worldName);
  auto s = std::ifstream(fileName.c_str(), std::ios::in | std::ios::binary);

  // compound start
  nbt::Tag rootTag = nbt::read_tag(s);
  nbt::skip_tag_payload(s, nbt::Tag::STRING);

  nbt::read_tagged_compound(s, rootTag, [this, &s](nbt::Tag tag, std::string name) {
    if (name == "GameTime") {
      gameTime = nbt::read_tagged_number<int>(s, tag);
    } else if (name == "Levels") {
      nbt::read_tagged_list(s, tag, nbt::Tag::COMPOUND, [this, &s](int i) {
        auto level = Level::deserialize(s);

        if (level.player) {
          player = level.player;
          currentLevel = i;
        }

        levels.push_back(level);
      });
    } else {
      nbt::skip_tag_payload(s, tag);
    }
  });

  sassert(!levels.empty(), "Save is missing Levels");
  sassert(player != nullptr, "Save is missing Player entity");
}