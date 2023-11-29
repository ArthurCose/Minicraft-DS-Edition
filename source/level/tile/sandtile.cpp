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
  std::array<uint8_t, 8> colors = {
    Color::get(level.sandColor - 110),
    Color::get(level.sandColor),
    Color::get(level.sandColor - 110),
    Color::get(level.dirtColor),
    Color::get(level.sandColor + 2),
    Color::get(level.sandColor - 110),
    Color::get(level.sandColor + 110),
  };

  bool u = !Tile::tiles[level.getTile(x, y - 1)]->connectsToSand;
  bool d = !Tile::tiles[level.getTile(x, y + 1)]->connectsToSand;
  bool l = !Tile::tiles[level.getTile(x - 1, y)]->connectsToSand;
  bool r = !Tile::tiles[level.getTile(x + 1, y)]->connectsToSand;

  bool steppedOn = level.getData(x, y) > 0;

  int offset = (r << 3) | (l << 2) | (d << 1) | u;

  if (steppedOn) {
    offset += 16;
  }

  screen.renderTile(x * 16, y * 16, offset, colors, 0);
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
