#pragma once

#include "../menu.h"

class AboutMenu : public Menu
{
public:
  void tick(Game &game) override;
  void render(Screen &game) override;
};
