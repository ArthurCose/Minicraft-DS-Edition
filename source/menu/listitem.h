#pragma once

#include "../gfx/screen.h"

class ListItem
{
public:
  virtual void renderInventory(Screen &screen, int i, int j){};
};
