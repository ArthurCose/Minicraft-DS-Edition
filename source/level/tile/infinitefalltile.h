#pragma once

#include "tile.h"

class InfiniteFallTile : public Tile
{
public:
  InfiniteFallTile(int id);

  int getMapColor(Level& level, int x, int y) override;
  bool mayPass(Level& level, int x, int y, Entity& e) override;
};
