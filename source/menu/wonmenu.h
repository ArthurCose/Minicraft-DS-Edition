#pragma once

#include "menu.h"
#include "resultsmenu.h"

class WonMenu : public ResultsMenu
{
public:
  WonMenu(int time, int score);

  void render(Screen &screen) override;
};
