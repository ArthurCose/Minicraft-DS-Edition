#pragma once

#include "tile.h"

class CactusTile : public Tile
{
public:
  CactusTile(int id);

  int getMapColor(Level& level, int x, int y) override;
  void render(Screen& screen, Level& level, int x, int y) override;
  bool mayPass(Level& level, int x, int y, Entity& e) override;
  void hurt(Level& level, int x, int y, Mob& source, int dmg, int attackDir) override;
  void bumpedInto(Level& level, int x, int y, Entity& entity) override;
  void tick(Level& level, int xt, int yt) override;
};
