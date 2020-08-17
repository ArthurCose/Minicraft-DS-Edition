#pragma once

#include "tile.h"

class GrassTile : public Tile
{
public:
  GrassTile(int id);

  void render(Screen &screen, Level &level, int x, int y) override;
  void tick(Level &level, int xt, int yt) override;
  bool interact(Level &level, int xt, int yt, Player &player, Item &item, int attackDir) override;
};