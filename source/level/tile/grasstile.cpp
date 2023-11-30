#include "grasstile.h"

#include "../../item/toolitem.h"
#include "../../item/resource/resource.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"
#include "../../sound/sound.h"

GrassTile::GrassTile(int id)
  : Tile(id)
{
  connectsToGrass = true;
}

int GrassTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(level.grassColor);
}

void GrassTile::render(Screen& screen, Level& level, int x, int y)
{
  auto colors = Color::getArray<8>({ level.grassColor - 111, level.grassColor, level.grassColor + 111, level.dirtColor });

  bool u = !Tile::tiles[level.getTile(x, y - 1)]->connectsToGrass;
  bool d = !Tile::tiles[level.getTile(x, y + 1)]->connectsToGrass;
  bool l = !Tile::tiles[level.getTile(x - 1, y)]->connectsToGrass;
  bool r = !Tile::tiles[level.getTile(x + 1, y)]->connectsToGrass;

  int offset = (r << 3) | (l << 2) | (d << 1) | u;

  screen.renderTile(x * 16, y * 16, offset, colors, 0);
}

void GrassTile::tick(Level& level, int xt, int yt)
{
  if (
    level.getTile(xt - 1, yt) != Tile::dirt &&
    level.getTile(xt + 1, yt) != Tile::dirt &&
    level.getTile(xt, yt - 1) != Tile::dirt &&
    level.getTile(xt, yt + 1) != Tile::dirt
    ) {
    // check for dirt tiles as random number gen is expensive
    return;
  }

  if (random.nextInt(40) != 0)
    return;

  int xn = xt;
  int yn = yt;

  if (random.nextBoolean())
    xn += random.nextBits(1) * 2 - 1;
  else
    yn += random.nextBits(1) * 2 - 1;

  if (level.getTile(xn, yn) == Tile::dirt) {
    level.setTile(xn, yn, this->id, 0);
  }
}

bool GrassTile::interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem*>(&item)) {
    if (tool->type == &ToolType::shovel) {
      if (player.payStamina(4 - tool->level)) {
        level.setTile(xt, yt, Tile::dirt, 0);
        Sound::monsterHurt.play();
        if (random.nextInt(5) == 0) {
          level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::seeds), xt * 16 + random.nextInt(10) + 3, yt * 16 + random.nextInt(10) + 3));
          return true;
        }
      }
    }

    if (tool->type == &ToolType::hoe) {
      if (player.payStamina(4 - tool->level)) {
        Sound::monsterHurt.play();
        if (random.nextInt(5) == 0) {
          level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::seeds), xt * 16 + random.nextInt(10) + 3, yt * 16 + random.nextInt(10) + 3));
          return true;
        }
        level.setTile(xt, yt, Tile::farmland, 0);
        return true;
      }
    }
  }
  return false;
}
