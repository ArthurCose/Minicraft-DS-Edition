#pragma once

#include "tile.h"

class SaplingTile : public Tile
{
private:
  unsigned char onType;
  unsigned char growsTo;

public:
  SaplingTile(int id, Tile* onType, Tile* growsTo);

  int getMapColor(Level& level, int x, int y) override;
  void tick(Level& level, int x, int y) override;
  void render(Screen& screen, Level& level, int x, int y) override;
  void hurt(Level& level, int x, int y, Mob& source, int dmg, int attackDir) override;
};
