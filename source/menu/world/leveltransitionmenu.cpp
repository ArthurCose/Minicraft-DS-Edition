#include "leveltransitionmenu.h"

#include "../player/ingamemenu.h"

LevelTransitionMenu::LevelTransitionMenu(int dir)
{
  this->dir = dir;
}

inline int calculateAddedBarHeightFromWidth(Screen& screen) {
  return ((screen.w + 7) / 8 + 2) / 3;
}

inline int calculateBarHeight(Screen& screen) {
  return screen.h / 8 + calculateAddedBarHeightFromWidth(screen);
}

void LevelTransitionMenu::tick(Game& game)
{
  time++;

  int height = calculateBarHeight(game.screen);
  int maxTime = height * 3 / 2;

  if (time == maxTime / 2 - 2)
    game.changeLevel(dir);
  if (time == maxTime)
    game.setMenu(std::make_unique<InGameMenu>(game.player, game.levels[game.currentLevel].map));
}

void LevelTransitionMenu::render(Screen& screen, Screen& bottomScreen)
{
  int height = calculateBarHeight(screen);
  int yOffset = height * 2;

  if (dir > 0) {
    yOffset -= height;
  }

  for (int i = 0; i < screen.w / 8; i++) {
    int dd = (i % 2 * 2 + i / 3) - time * 2;
    int x = i * 8;
    int y = (dd + yOffset) * 8;

    if (dir < 0) {
      y = screen.h - y - 8;
    }

    screen.renderBoxFilled(x, y, 8, height * 8, 0);
  }
}
