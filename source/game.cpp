#include "game.h"
#include "gfx/color.h"
#include "menu/main/titlemenu.h"
#include "menu/world/leveltransitionmenu.h"
#include "menu/player/deadmenu.h"
#include "menu/player/wonmenu.h"
#include "icons.h"

const std::string Game::NAME = "Minicraft DS";

Game::Game()
{
  setMenu(std::make_unique<TitleMenu>());
}

void Game::tick()
{
  tickCount++;

  scanKeys();

  bool blockTick = false;

  if (menu != NULL)
  {
    blockTick = menu->blocksGameTick;
    menu->tick(*this);
  }

  if (blockTick)
    return;

  if (player->removed)
  {
    playerDeadTime++;

    if (playerDeadTime > 60)
    {
      setMenu(std::make_unique<DeadMenu>(gameTime, player->score));
    }
  }
  else
  {
    if (!hasWon)
      gameTime++;

    if (pendingLevelChange != 0)
    {
      setMenu(std::make_unique<LevelTransitionMenu>(pendingLevelChange));
      pendingLevelChange = 0;
    }
  }

  if (wonTimer > 0)
  {
    if (--wonTimer == 0)
    {
      setMenu(std::make_unique<WonMenu>(gameTime, player->score));
    }
  }

  levels[currentLevel].tick(*this);
  Tile::tickCount++;
}

bool Game::isHeld(int key)
{
  return keysHeld() & key;
}

bool Game::justTapped(int key)
{
  return keysDown() & key;
}

void Game::render()
{
  if (player != NULL)
  {
    Level &level = levels[currentLevel];

    level.render(screen, lightMask, *player);
  }

  if (menu != NULL)
  {
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
  player = NULL;
  playerDeadTime = 0;
  pendingLevelChange = 0;
  wonTimer = 0;
  gameTime = 0;
  hasWon = false;
}
