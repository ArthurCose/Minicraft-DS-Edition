#pragma once

#include "menu.h"

class TitleMenu : public Menu
{
private:
  int selected = 0;

public:
  virtual void tick(Game &game) override;
  virtual void render(Screen &screen) override;
};
