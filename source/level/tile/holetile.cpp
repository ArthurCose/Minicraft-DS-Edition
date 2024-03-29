#include "holetile.h"

#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

HoleTile::HoleTile(int id)
  : Tile(id)
{
  connectsToSand = true;
  connectsToWater = true;
  connectsToLava = true;
}

int HoleTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(111);
}

void HoleTile::render(Screen& screen, Level& level, int x, int y)
{
  int transitionColor1 = Color::get(3, 111, level.dirtColor - 111, level.dirtColor);
  int transitionColor2 = Color::get(3, 111, level.sandColor - 110, level.sandColor);

  bool u = !Tile::tiles[level.getTile(x, y - 1)]->connectsToLiquid();
  bool d = !Tile::tiles[level.getTile(x, y + 1)]->connectsToLiquid();
  bool l = !Tile::tiles[level.getTile(x - 1, y)]->connectsToLiquid();
  bool r = !Tile::tiles[level.getTile(x + 1, y)]->connectsToLiquid();

  bool su = u && Tile::tiles[level.getTile(x, y - 1)]->connectsToSand;
  bool sd = d && Tile::tiles[level.getTile(x, y + 1)]->connectsToSand;
  bool sl = l && Tile::tiles[level.getTile(x - 1, y)]->connectsToSand;
  bool sr = r && Tile::tiles[level.getTile(x + 1, y)]->connectsToSand;

  auto colors = Color::getArray<8>({ 111, 111, 110, 110 });

  if (!(u && d && l && r)) {
    screen.renderTile(x * 16, y * 16, 0, colors, 0);
  }

  if (u || l) {
    screen.renderIcon(x * 16 + 0, y * 16 + 0, (l ? 14 : 15) + (u ? 0 : 1) * 32, (su || sl) ? transitionColor2 : transitionColor1, 0);
  }

  if (u || r) {
    screen.renderIcon(x * 16 + 8, y * 16 + 0, (r ? 16 : 15) + (u ? 0 : 1) * 32, (su || sr) ? transitionColor2 : transitionColor1, 0);
  }

  if (d || l) {
    screen.renderIcon(x * 16 + 0, y * 16 + 8, (l ? 14 : 15) + (d ? 2 : 1) * 32, (sd || sl) ? transitionColor2 : transitionColor1, 0);
  }

  if (d || r) {
    screen.renderIcon(x * 16 + 8, y * 16 + 8, (r ? 16 : 15) + (d ? 2 : 1) * 32, (sd || sr) ? transitionColor2 : transitionColor1, 0);
  }
}

bool HoleTile::mayPass(Level& level, int x, int y, Entity& e)
{
  return e.canSwim();
}
