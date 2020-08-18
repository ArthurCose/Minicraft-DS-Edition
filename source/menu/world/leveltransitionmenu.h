#pragma once

#include "../menu.h"

class LevelTransitionMenu : public Menu
{
private:
  int dir;
  int time = 0;

public:
  LevelTransitionMenu(int dir);

  void tick(Game &game) override;
  void render(Screen &screen) override;
};
