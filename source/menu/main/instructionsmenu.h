#pragma once

#include "../menu.h"

class InstructionsMenu : public Menu
{
public:
  void tick(Game& game) override;
  void render(Screen& screen, Screen& bottomScreen) override;
};
