#pragma once

#include "tile.h"

class WaterTile : public Tile
{
private:
  Random wRandom;

public:
  WaterTile(int id);

  void tick(Level &level, int xt, int yt) override;
  void render(Screen &screen, Level &level, int x, int y) override;
  bool mayPass(Level &level, int x, int y, Entity &e) override;
};
