#include "resultsmenu.h"

#include "../main/titlemenu.h"

ResultsMenu::ResultsMenu(int time, int score)
{
  int seconds = time / 60;
  int minutes = seconds / 60;
  int hours = minutes / 60;
  minutes %= 60;
  seconds %= 60;

  if (hours > 0) {
    timeString = std::to_string(hours) + "h";

    if (minutes < 10)
      timeString += "0";
    timeString += std::to_string(minutes) + "m";
  } else {
    timeString = std::to_string(minutes) + "m ";

    if (seconds < 10)
      timeString += "0";
    timeString += std::to_string(seconds) + "s";
  }

  scoreString = std::to_string(score);
}

void ResultsMenu::tick(Game& game)
{
  if (inputDelay > 0)
    inputDelay--;
  else if (game.justTapped(KEY_START)) {
    game.resetGame();
    game.setMenu(std::make_unique<TitleMenu>());
  }
}

void ResultsMenu::renderResults(Screen& screen, std::string_view message, std::string_view closeMessage)
{
  int x = 5;
  int y = 8;

  screen.renderFrame("", x, y, x + 20, y + 6);

  screen.renderText(message, (x + 1) * 8, (y + 1) * 8, Color::get(-1, 555, 555, 555));

  int timeY = (y + 2) * 8;
  screen.renderText("Time:", (x + 1) * 8, timeY, Color::get(-1, 555, 555, 555));
  screen.renderText(timeString, (x + 1 + 5) * 8, timeY, Color::get(-1, 550, 550, 550));

  int scoreY = (y + 3) * 8;
  screen.renderText("Score:", (x + 1) * 8, scoreY, Color::get(-1, 555, 555, 555));
  screen.renderText(scoreString, (x + 1 + 6) * 8, scoreY, Color::get(-1, 550, 550, 550));

  screen.renderText(closeMessage, (x + 1) * 8, (y + 5) * 8, Color::get(-1, 333, 333, 333));
}
