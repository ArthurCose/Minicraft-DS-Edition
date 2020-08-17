#pragma once

#include "tile.h"

class InfiniteFallTile : public Tile
{
public:
  InfiniteFallTile(int id);

  bool mayPass(Level &level, int x, int y, Entity &e) override;
};
