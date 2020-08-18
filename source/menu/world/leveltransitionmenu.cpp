#include "leveltransitionmenu.h"

LevelTransitionMenu::LevelTransitionMenu(int dir)
{
  this->dir = dir;
}

void LevelTransitionMenu::tick(Game &game)
{
  time += 2;
  if (time == 30)
    game.changeLevel(dir);
  if (time == 60)
    close(game);
}

void LevelTransitionMenu::render(Screen &screen)
{
  for (int x = 0; x < 20; x++)
  {
    for (int y = 0; y < 15; y++)
    {
      int dd = (y + x % 2 * 2 + x / 3) - time;
      if (dd < 0 && dd > -30)
      {
        if (dir > 0)
          screen.renderTile(x * 8, y * 8, 0, 0, 0);
        else
          screen.renderTile(x * 8, screen.h - y * 8 - 8, 0, 0, 0);
      }
    }
  }
}
