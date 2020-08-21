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

int HoleTile::getMapColor(Level &level, int x, int y)
{
  return Color::get(111);
}

void HoleTile::render(Screen &screen, Level &level, int x, int y)
{
  int col = Color::get(111, 111, 110, 110);
  int transitionColor1 = Color::get(3, 111, level.dirtColor - 111, level.dirtColor);
  int transitionColor2 = Color::get(3, 111, level.sandColor - 110, level.sandColor);

  bool u = !level.getTile(x, y - 1)->connectsToLiquid();
  bool d = !level.getTile(x, y + 1)->connectsToLiquid();
  bool l = !level.getTile(x - 1, y)->connectsToLiquid();
  bool r = !level.getTile(x + 1, y)->connectsToLiquid();

  bool su = u && level.getTile(x, y - 1)->connectsToSand;
  bool sd = d && level.getTile(x, y + 1)->connectsToSand;
  bool sl = l && level.getTile(x - 1, y)->connectsToSand;
  bool sr = r && level.getTile(x + 1, y)->connectsToSand;

  if (!u && !l)
  {
    screen.renderTile(x * 16 + 0, y * 16 + 0, 0, col, 0);
  }
  else
    screen.renderTile(x * 16 + 0, y * 16 + 0, (l ? 14 : 15) + (u ? 0 : 1) * 32, (su || sl) ? transitionColor2 : transitionColor1, 0);

  if (!u && !r)
  {
    screen.renderTile(x * 16 + 8, y * 16 + 0, 1, col, 0);
  }
  else
    screen.renderTile(x * 16 + 8, y * 16 + 0, (r ? 16 : 15) + (u ? 0 : 1) * 32, (su || sr) ? transitionColor2 : transitionColor1, 0);

  if (!d && !l)
  {
    screen.renderTile(x * 16 + 0, y * 16 + 8, 2, col, 0);
  }
  else
    screen.renderTile(x * 16 + 0, y * 16 + 8, (l ? 14 : 15) + (d ? 2 : 1) * 32, (sd || sl) ? transitionColor2 : transitionColor1, 0);
  if (!d && !r)
  {
    screen.renderTile(x * 16 + 8, y * 16 + 8, 3, col, 0);
  }
  else
    screen.renderTile(x * 16 + 8, y * 16 + 8, (r ? 16 : 15) + (d ? 2 : 1) * 32, (sd || sr) ? transitionColor2 : transitionColor1, 0);
}

bool HoleTile::mayPass(Level &level, int x, int y, Entity &e)
{
  return e.canSwim();
}
