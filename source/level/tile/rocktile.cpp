#include "rocktile.h"

#include "../../item/toolitem.h"
#include "../../entity/particle/smashparticle.h"
#include "../../entity/particle/textparticle.h"

RockTile::RockTile(int id) : Tile(id) {}

void RockTile::tick(Level& level, int xt, int yt)
{
  int damage = level.getData(xt, yt);
  if (damage > 0)
    level.setData(xt, yt, damage - 1);
}

int RockTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(444);
}

void RockTile::render(Screen& screen, Level& level, int x, int y)
{
  int transitionColor = Color::get(111, 444, 555, level.dirtColor);

  bool u = level.getTile(x, y - 1) != this->id;
  bool d = level.getTile(x, y + 1) != this->id;
  bool l = level.getTile(x - 1, y) != this->id;
  bool r = level.getTile(x + 1, y) != this->id;

  bool ul = level.getTile(x - 1, y - 1) != this->id;
  bool dl = level.getTile(x - 1, y + 1) != this->id;
  bool ur = level.getTile(x + 1, y - 1) != this->id;
  bool dr = level.getTile(x + 1, y + 1) != this->id;

  int offset = (r << 3) | (l << 2) | (d << 1) | u;

  auto colors = Color::getArray<8>({ 111, 444, 333, 555, level.dirtColor });

  screen.renderTile(x * 16, y * 16, 3 * 16 + offset, colors, 0);

  if (!u && !l && ul) {
    screen.renderIcon(x * 16 + 0, y * 16 + 0, 7 + 0 * 32, transitionColor, 3);
  }

  if (!u && !r && ur) {
    screen.renderIcon(x * 16 + 8, y * 16 + 0, 8 + 0 * 32, transitionColor, 3);
  }

  if (!d && !l && dl) {
    screen.renderIcon(x * 16 + 0, y * 16 + 8, 7 + 1 * 32, transitionColor, 3);
  }

  if (!d && !r && dr) {
    screen.renderIcon(x * 16 + 8, y * 16 + 8, 8 + 1 * 32, transitionColor, 3);
  }
}

bool RockTile::mayPass(Level& level, int x, int y, Entity& e)
{
  return false;
}

bool RockTile::interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem*>(&item)) {
    if (tool->type == &ToolType::pickaxe) {
      if (player.payStamina(4 - tool->level)) {
        hurt(level, xt, yt, random.nextInt(10) + (tool->level) * 5 + 10);
        return true;
      }
    }
  }
  return false;
}

void RockTile::hurt(Level& level, int x, int y, Mob& source, int dmg, int attackDir)
{
  hurt(level, x, y, dmg);
}

void RockTile::hurt(Level& level, int x, int y, int dmg)
{
  int damage = level.getData(x, y) + dmg;
  level.add(std::make_shared<SmashParticle>(x * 16 + 8, y * 16 + 8));
  level.add(std::make_shared<TextParticle>(std::to_string(dmg), x * 16 + 8, y * 16 + 8, Color::get(-1, 500, 500, 500)));

  if (damage >= 50) {
    int count = random.nextInt(4) + 1;
    for (int i = 0; i < count; i++) {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::stone), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
    count = random.nextInt(2);
    for (int i = 0; i < count; i++) {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::coal), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
    level.setTile(x, y, Tile::dirt, 0);
  } else {
    level.setData(x, y, damage);
  }
}
