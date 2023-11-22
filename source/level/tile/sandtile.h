#pragma once

#include "tile.h"

class SandTile : public Tile
{
public:
  SandTile(int id);

  int getMapColor(Level& level, int x, int y) override;
  void render(Screen& screen, Level& level, int x, int y) override;
  void tick(Level& level, int x, int y) override;
  void steppedOn(Level& level, int x, int y, Entity& entity) override;
  bool interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir) override;
};
