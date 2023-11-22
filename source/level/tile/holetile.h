#pragma once

#include "tile.h"

class HoleTile : public Tile
{
public:
  HoleTile(int id);

  int getMapColor(Level& level, int x, int y) override;
  void render(Screen& screen, Level& level, int x, int y) override;
  bool mayPass(Level& level, int x, int y, Entity& e) override;
};
