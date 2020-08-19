#pragma once

#include "menu/menu.h"
#include "level/level.h"
#include "entity/player.h"
#include "gfx/lightmask.h"

#include <memory>

class Menu;

class Game
{
public:
  static const std::string NAME;
  Screen screen;
  std::shared_ptr<Player> player;
  std::unique_ptr<Menu> menu;
  std::vector<Level> levels;
  int currentLevel = 3;

  Game();

  void tick();
  bool isHeld(int key);
  bool justTapped(int key);
  void render();
  void setMenu(std::unique_ptr<Menu> menu);
  void enterMenu(std::unique_ptr<Menu> menu);
  void scheduleLevelChange(int dir);
  void changeLevel(int dir);
  void win();
  void resetGame();

private:
  Screen lightScreen;
  LightMask lightMask;
  static const int SCALE = 3;
  int tickCount = 0;
  int gameTime = 0;
  int playerDeadTime;
  int pendingLevelChange;
  int wonTimer = 0;
  bool hasWon = false;
  void init();
  void renderHud();
};