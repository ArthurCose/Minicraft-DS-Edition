#pragma once

#include "tile.h"

class CloudTile : public Tile
{
public:
  CloudTile(int id);

  int getMapColor(Level& level, int x, int y) override;
  void render(Screen& screen, Level& level, int x, int y) override;
  bool mayPass(Level& level, int x, int y, Entity& e) override;
  bool interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir) override;
};
