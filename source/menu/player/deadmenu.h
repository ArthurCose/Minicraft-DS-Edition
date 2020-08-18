#pragma once

#include "resultsmenu.h"

class DeadMenu : public ResultsMenu
{
public:
  DeadMenu(int time, int score);

  void render(Screen &screen) override;
};
