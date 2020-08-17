#pragma once

#include "tile.h"

class DirtTile : public Tile
{
public:
  DirtTile(int id);

  void render(Screen &screen, Level &level, int x, int y) override;
  bool interact(Level &level, int xt, int yt, Player &player, Item &item, int attackDir) override;
};
