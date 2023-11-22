#include "leveltransitionmenu.h"

#include "../player/ingamemenu.h"

LevelTransitionMenu::LevelTransitionMenu(int dir)
{
  this->dir = dir;
}

void LevelTransitionMenu::tick(Game& game)
{
  time += 2;
  if (time == 30)
    game.changeLevel(dir);
  if (time == 60)
    game.setMenu(std::make_unique<InGameMenu>(game.player, game.levels[game.currentLevel].map));
}

void LevelTransitionMenu::render(Screen& screen, Screen& bottomScreen)
{
  for (int x = 0; x < screen.w / 8; x++) {
    for (int y = 0; y < screen.h / 8; y++) {
      int dd = (y + x % 2 * 2 + x / 3) - time;
      if (dd < 0 && dd > -30) {
        if (dir > 0)
          screen.renderTile(x * 8, y * 8, 0, 0, 0);
        else
          screen.renderTile(x * 8, screen.h - y * 8 - 8, 0, 0, 0);
      }
    }
  }
}
