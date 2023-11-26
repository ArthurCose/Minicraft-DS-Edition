#pragma once

#include "menu/menu.h"
#include "level/level.h"
#include "entity/player.h"
#include "gfx/glscreen.h"
#include "gfx/softwarescreen.h"
#include "gfx/lightmask.h"

#include <memory>

class Menu;

class Game
{
public:
  static const std::string NAME;
  static const std::string SAVE_FOLDER;
  static const std::string SAVE_EXTENSION;

  GLScreen screen;
  SoftwareScreen bottomScreen;
  std::shared_ptr<Player> player;
  std::unique_ptr<Menu> menu;
  std::string worldName;
  std::vector<Level> levels;
  int currentLevel = 3;
  bool frameSkipEnabled = true;

  Game();

  void tick();
  bool isHeld(int key);
  bool justTapped(int key);
  bool justTappedOrRepeat(int key);
  bool justReleased(int key);
  int touchX();
  int touchY();
  void render();
  void setMenu(std::unique_ptr<Menu> menu);
  void enterMenu(std::unique_ptr<Menu> menu);
  void scheduleLevelChange(int dir);
  void changeLevel(int dir);
  void win();
  void resetGame();
  static std::string savePath(const std::string& name);
  void resolveNameConflict();
  void save();
  void load(std::string worldName);

private:
  LightMask lightMask;
  static const int SCALE = 3;
  touchPosition touchPos;
  int tappedKeys = 0;
  int heldKeys = 0;
  int tappedOrRepeatKeys = 0;
  int releasedKeys = 0;
  int tickCount = 0;
  int gameTime = 0;
  int playerDeadTime;
  int pendingLevelChange = 0;
  int wonTimer = 0;
  bool hasWon = false;
};