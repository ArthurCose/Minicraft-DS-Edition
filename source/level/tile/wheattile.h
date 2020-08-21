#pragma once

#include "tile.h"

class WheatTile : public Tile
{
public:
  WheatTile(int id);

  int getMapColor(Level &level, int x, int y) override;
  void tick(Level &level, int xt, int yt) override;
  void render(Screen &screen, Level &level, int x, int y) override;
  bool interact(Level &level, int xt, int yt, Player &player, Item &item, int attackDir) override;
  void steppedOn(Level &level, int xt, int yt, Entity &entity) override;
  void hurt(Level &level, int x, int y, Mob &source, int dmg, int attackDir) override;

private:
  void harvest(Level &level, int x, int y);
};
