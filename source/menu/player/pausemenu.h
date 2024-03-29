#pragma once

#include "../menu.h"

class PauseMenu : public Menu
{
public:
  void tick(Game& game) override;
  void render(Screen& screen, Screen& bottomScreen) override;

private:
  int selected = 0;
  bool saving = false;
};
