#pragma once

#include "tile.h"

class LavaTile : public Tile
{
private:
  Random wRandom;

public:
  LavaTile(int id);

  int getMapColor(Level &level, int x, int y) override;
  void render(Screen &screen, Level &level, int x, int y) override;
  bool mayPass(Level &level, int x, int y, Entity &e) override;
  void tick(Level &level, int xt, int yt) override;
  int getLightRadius(Level &level, int x, int y) override;
};
