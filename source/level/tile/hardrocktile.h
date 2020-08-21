#pragma once

#include "tile.h"

class HardRockTile : public Tile
{
public:
  HardRockTile(int id);

  int getMapColor(Level &level, int x, int y) override;
  void tick(Level &level, int xt, int yt) override;
  void render(Screen &screen, Level &level, int x, int y) override;
  bool mayPass(Level &level, int x, int y, Entity &e) override;
  void hurt(Level &level, int x, int y, Mob &source, int dmg, int attackDir) override;
  bool interact(Level &level, int xt, int yt, Player &player, Item &item, int attackDir) override;

private:
  void hurt(Level &level, int x, int y, int dmg);
};
