#pragma once

#include "../menu.h"

class TitleMenu : public Menu
{
private:
  int selected = 0;
  bool clearedSubScreen = false;

public:
  virtual void tick(Game &game) override;
  virtual void render(Screen &screen, Screen &bottomScreen) override;
};
