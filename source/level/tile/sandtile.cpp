#include "sandtile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

SandTile::SandTile(int id) : Tile(id)
{
  connectsToSand = true;
}

int SandTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(level.sandColor);
}

void SandTile::render(Screen& screen, Level& level, int x, int y)
{
  int col = Color::get(level.sandColor + 2, level.sandColor, level.sandColor - 110, level.sandColor - 110);
  int transitionColor = Color::get(level.sandColor - 110, level.sandColor, level.sandColor - 110, level.dirtColor);

  bool u = !Tile::tiles[level.getTile(x, y - 1)]->connectsToSand;
  bool d = !Tile::tiles[level.getTile(x, y + 1)]->connectsToSand;
  bool l = !Tile::tiles[level.getTile(x - 1, y)]->connectsToSand;
  bool r = !Tile::tiles[level.getTile(x + 1, y)]->connectsToSand;

  bool steppedOn = level.getData(x, y) > 0;

  if (!u && !l) {
    if (!steppedOn)
      screen.renderTile(x * 16 + 0, y * 16 + 0, 0, col, 0);
    else
      screen.renderTile(x * 16 + 0, y * 16 + 0, 3 + 1 * 32, col, 0);
  } else
    screen.renderTile(x * 16 + 0, y * 16 + 0, (l ? 11 : 12) + (u ? 0 : 1) * 32, transitionColor, 0);

  if (!u && !r) {
    screen.renderTile(x * 16 + 8, y * 16 + 0, 1, col, 0);
  } else
    screen.renderTile(x * 16 + 8, y * 16 + 0, (r ? 13 : 12) + (u ? 0 : 1) * 32, transitionColor, 0);

  if (!d && !l) {
    screen.renderTile(x * 16 + 0, y * 16 + 8, 2, col, 0);
  } else
    screen.renderTile(x * 16 + 0, y * 16 + 8, (l ? 11 : 12) + (d ? 2 : 1) * 32, transitionColor, 0);
  if (!d && !r) {
    if (!steppedOn)
      screen.renderTile(x * 16 + 8, y * 16 + 8, 3, col, 0);
    else
      screen.renderTile(x * 16 + 8, y * 16 + 8, 3 + 1 * 32, col, 0);
  } else
    screen.renderTile(x * 16 + 8, y * 16 + 8, (r ? 13 : 12) + (d ? 2 : 1) * 32, transitionColor, 0);
}

void SandTile::tick(Level& level, int x, int y)
{
  int d = level.getData(x, y);
  if (d > 0)
    level.setData(x, y, d - 1);
}

void SandTile::steppedOn(Level& level, int x, int y, Entity& entity)
{
  if (dynamic_cast<Mob*>(&entity)) {
    level.setData(x, y, 10);
  }
}

bool SandTile::interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem*>(&item)) {
    if (tool->type == &ToolType::shovel) {
      if (player.payStamina(4 - tool->level)) {
        level.setTile(xt, yt, Tile::dirt, 0);
        level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::sand), xt * 16 + random.nextInt(10) + 3, yt * 16 + random.nextInt(10) + 3));
        return true;
      }
    }
  }
  return false;
}
