#pragma once

#include "grasstile.h"

class FlowerTile : public GrassTile
{
public:
  FlowerTile(int id);

  void render(Screen &screen, Level &level, int x, int y) override;
  bool interact(Level &level, int x, int y, Player &player, Item &item, int attackDir) override;
  void hurt(Level &level, int x, int y, Mob &source, int dmg, int attackDir) override;
};
