#include "wheattile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

WheatTile::WheatTile(int id) : Tile(id) {}

int WheatTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(450);
}

void WheatTile::tick(Level& level, int xt, int yt)
{
  if (random.nextBoolean())
    return;

  int age = level.getData(xt, yt);
  if (age < 50)
    level.setData(xt, yt, age + 1);
}

void WheatTile::render(Screen& screen, Level& level, int x, int y)
{
  int age = level.getData(x, y);

  auto colors = Color::getArray<8>({ level.dirtColor - 121, level.dirtColor - 11, level.dirtColor, 50 });

  int icon = age / 10;

  if (icon >= 3) {
    colors[2] = Color::get(50 + (icon) * 100);
    colors[3] = Color::get(40 + (icon - 3) * 2 * 100);

    if (age == 50) {
      colors[0] = 0;
      colors[1] = 0;
    }

    icon = 3;
  }

  screen.renderTile(x * 16, y * 16, 6 * 16 + 8 + icon, colors, 0);
}

bool WheatTile::interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem*>(&item)) {
    if (tool->type == &ToolType::shovel) {
      if (player.payStamina(4 - tool->level)) {
        level.setTile(xt, yt, Tile::dirt, 0);
        return true;
      }
    }
  }
  return false;
}

void WheatTile::steppedOn(Level& level, int xt, int yt, Entity& entity)
{
  if (random.nextInt(60) != 0)
    return;
  if (level.getData(xt, yt) < 2)
    return;
  harvest(level, xt, yt);
}

void WheatTile::hurt(Level& level, int x, int y, Mob& source, int dmg, int attackDir)
{
  harvest(level, x, y);
}

void WheatTile::harvest(Level& level, int x, int y)
{
  int age = level.getData(x, y);

  int count = random.nextInt(2);
  for (int i = 0; i < count; i++) {
    level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::seeds), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
  }

  count = 0;
  if (age == 50) {
    count = random.nextInt(3) + 2;
  } else if (age >= 40) {
    count = random.nextInt(2) + 1;
  }
  for (int i = 0; i < count; i++) {
    level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::wheat), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
  }

  level.setTile(x, y, Tile::dirt, 0);
}
