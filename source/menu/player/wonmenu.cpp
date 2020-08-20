#include "wonmenu.h"

WonMenu::WonMenu(int time, int score)
    : ResultsMenu(time, score) {}

void WonMenu::render(Screen &screen, Screen &bottomScreen)
{
  ResultsMenu::render(screen, bottomScreen);
  screen.renderText("You won! Yay!", 2 * 8, 4 * 8, Color::get(-1, 555, 555, 555));

  screen.renderText("Press START to quit", 2 * 8, 8 * 8, Color::get(-1, 333, 333, 333));
}
