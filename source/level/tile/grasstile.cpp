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
  int col = Color::get(level.grassColor, level.grassColor, level.grassColor + 111, level.grassColor + 111);
  int transitionColor = Color::get(level.grassColor - 111, level.grassColor, level.grassColor + 111, level.dirtColor);

  bool u = !Tile::tiles[level.getTile(x, y - 1)]->connectsToGrass;
  bool d = !Tile::tiles[level.getTile(x, y + 1)]->connectsToGrass;
  bool l = !Tile::tiles[level.getTile(x - 1, y)]->connectsToGrass;
  bool r = !Tile::tiles[level.getTile(x + 1, y)]->connectsToGrass;

  if (!u && !l) {
    screen.renderTile(x * 16 + 0, y * 16 + 0, 0, col, 0);
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
    screen.renderTile(x * 16 + 8, y * 16 + 8, 3, col, 0);
  } else
    screen.renderTile(x * 16 + 8, y * 16 + 8, (r ? 13 : 12) + (d ? 2 : 1) * 32, transitionColor, 0);
}

void GrassTile::tick(Level& level, int xt, int yt)
{
  if (random.nextInt(40) != 0)
    return;

  int xn = xt;
  int yn = yt;

  if (random.nextBoolean())
    xn += random.nextInt(2) * 2 - 1;
  else
    yn += random.nextInt(2) * 2 - 1;

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
