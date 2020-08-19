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

  if (menu != NULL)
  {
    menu->tick(*this);
  }
  else
  {
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

    renderHud();
  }

  if (menu != NULL)
  {
    menu->render(screen);
  }
}

void Game::renderHud()
{
  const int hudTop = screen.h - 8;
  const int staminaBarLeft = screen.w - 10 * 8;

  for (int i = 0; i < 10; i++)
  {
    if (i < player->health)
      screen.renderTile(i * 8, hudTop, 0 + 12 * 32, Color::get(000, 200, 500, 533), 0);
    else
      screen.renderTile(i * 8, hudTop, 0 + 12 * 32, Color::get(000, 100, 000, 000), 0);

    if (player->staminaRechargeDelay > 0)
    {
      if (player->staminaRechargeDelay / 4 % 2 == 0)
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 555, 000, 000), 0);
      else
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 110, 000, 000), 0);
    }
    else
    {
      if (i < player->stamina)
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 220, 550, 553), 0);
      else
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 110, 000, 000), 0);
    }
  }

  if (player->activeItem != NULL)
  {
    player->activeItem->renderInventory(screen, 10 * 8, screen.h - 16);
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
