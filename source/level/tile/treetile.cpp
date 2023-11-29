#include "treetile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../entity/particle/smashparticle.h"
#include "../../entity/particle/textparticle.h"
#include "../../gfx/screen.h"

TreeTile::TreeTile(int id) : Tile(id)
{
  connectsToGrass = true;
}

int TreeTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(30);
}

void TreeTile::render(Screen& screen, Level& level, int x, int y)
{
  std::array<uint8_t, 8> colors = {
    Color::get(10),
    Color::get(30),
    Color::get(320),
    Color::get(430),
    Color::get(151),
    Color::get(level.grassColor),
  };

  bool u = level.getTile(x, y - 1) == this->id;
  bool l = level.getTile(x - 1, y) == this->id;
  bool r = level.getTile(x + 1, y) == this->id;
  bool d = level.getTile(x, y + 1) == this->id;
  bool ul = level.getTile(x - 1, y - 1) == this->id;
  bool ur = level.getTile(x + 1, y - 1) == this->id;
  bool dl = level.getTile(x - 1, y + 1) == this->id;
  bool dr = level.getTile(x + 1, y + 1) == this->id;

  int offset0 = u && ul && l;
  int offset1 = u && ur && r;
  int offset2 = d && dl && l;
  int offset3 = d && dr && r;
  int offset = (offset3 << 3) | (offset2 << 2) | (offset1 << 1) | offset0;

  screen.renderTile(x * 16, y * 16, 5 * 16 + offset, colors, 0);
}

void TreeTile::tick(Level& level, int xt, int yt)
{
  int damage = level.getData(xt, yt);
  if (damage > 0)
    level.setData(xt, yt, damage - 1);
}

bool TreeTile::mayPass(Level& level, int x, int y, Entity& e)
{
  return false;
}

void TreeTile::hurt(Level& level, int x, int y, Mob& source, int dmg, int attackDir)
{
  hurt(level, x, y, dmg);
}

bool TreeTile::interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem*>(&item)) {
    if (tool->type == &ToolType::axe) {
      if (player.payStamina(4 - tool->level)) {
        hurt(level, xt, yt, random.nextInt(10) + (tool->level) * 5 + 10);
        return true;
      }
    }
  }
  return false;
}

void TreeTile::hurt(Level& level, int x, int y, int dmg)
{
  {
    int count = random.nextInt(10) == 0 ? 1 : 0;
    for (int i = 0; i < count; i++) {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::apple), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
  }
  int damage = level.getData(x, y) + dmg;
  level.add(std::make_shared<SmashParticle>(x * 16 + 8, y * 16 + 8));
  level.add(std::make_shared<TextParticle>(std::to_string(dmg), x * 16 + 8, y * 16 + 8, Color::get(-1, 500, 500, 500)));
  if (damage >= 20) {
    int count = random.nextInt(2) + 1;
    for (int i = 0; i < count; i++) {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::wood), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
    count = random.nextInt(random.nextInt(4) + 1);
    for (int i = 0; i < count; i++) {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::acorn), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
    level.setTile(x, y, Tile::grass, 0);
  } else {
    level.setData(x, y, damage);
  }
}
