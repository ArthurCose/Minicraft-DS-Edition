#include "resultsmenu.h"

#include "../main/titlemenu.h"

ResultsMenu::ResultsMenu(int time, int score)
{
  int seconds = time / 60;
  int minutes = seconds / 60;
  int hours = minutes / 60;
  minutes %= 60;
  seconds %= 60;

  if (hours > 0)
  {
    timeString = std::to_string(hours) + "h";

    if (minutes < 10)
      timeString += "0";
    timeString += std::to_string(minutes) + "m";
  }
  else
  {
    timeString = std::to_string(minutes) + "m ";

    if (seconds < 10)
      timeString += "0";
    timeString += std::to_string(seconds) + "s";
  }

  scoreString = std::to_string(score);
}

void ResultsMenu::tick(Game &game)
{
  if (inputDelay > 0)
    inputDelay--;
  else if (game.justTapped(KEY_START))
  {
    game.setMenu(std::make_unique<TitleMenu>());
  }
}

void ResultsMenu::render(Screen &screen)
{
  screen.renderFrame("", 1, 3, 21, 9);

  screen.renderText("Time:", 2 * 8, 5 * 8, Color::get(-1, 555, 555, 555));
  screen.renderText(timeString, (2 + 5) * 8, 5 * 8, Color::get(-1, 550, 550, 550));
  screen.renderText("Score:", 2 * 8, 6 * 8, Color::get(-1, 555, 555, 555));
  screen.renderText(scoreString, (2 + 6) * 8, 6 * 8, Color::get(-1, 550, 550, 550));
}
