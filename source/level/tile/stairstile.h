#pragma once

#include "tile.h"

class StairsTile : public Tile
{
public:
  StairsTile(int id, bool leadsUp);

  void render(Screen &screen, Level &level, int x, int y) override;

private:
  bool leadsUp;
};
