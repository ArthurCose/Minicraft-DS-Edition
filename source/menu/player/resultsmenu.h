#pragma once

#include "../menu.h"

// base class for DeadMenu and WonMenu
class ResultsMenu : public Menu
{
private:
  int inputDelay = 60;
  std::string timeString;
  std::string scoreString;

protected:
  void renderResults(Screen& screen, std::string_view message, std::string_view closeMessage);

public:
  ResultsMenu(int time, int score);

  void tick(Game& game) override;
};
