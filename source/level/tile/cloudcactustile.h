#pragma once

#include "tile.h"

class CloudCactusTile : public Tile
{
public:
  CloudCactusTile(int id);

  void render(Screen &screen, Level &level, int x, int y) override;
  bool mayPass(Level &level, int x, int y, Entity &e) override;
  void hurt(Level &level, int x, int y, Mob &source, int dmg, int attackDir) override;
  bool interact(Level &level, int xt, int yt, Player &player, Item &item, int attackDir) override;
  void bumpedInto(Level &level, int x, int y, Entity &entity) override;

private:
  void hurt(Level &level, int x, int y, int dmg);
};
