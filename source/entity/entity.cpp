#include "entity.h"

#include <ctime>
#include <algorithm>
#include <vector>
#include "../level/level.h"
#include "../nbt.h"
#include "hostile/airwizard.h"
#include "hostile/slime.h"
#include "hostile/zombie.h"
#include "lighting/lantern.h"
#include "projectile/spark.h"
#include "storage/chest.h"
#include "workbenches/anvil.h"
#include "workbenches/furnace.h"
#include "workbenches/oven.h"
#include "workbenches/workbench.h"
#include "../item/powergloveitem.h"

Random Entity::random(time(NULL));

void Entity::remove()
{
  removed = true;
}

bool Entity::intersects(int x0, int y0, int x1, int y1)
{
  return !(x + xr < x0 || y + yr < y0 || x - xr > x1 || y - yr > y1);
}

bool Entity::move(Level& level, int xa, int ya)
{
  if (xa != 0 || ya != 0) {
    bool stopped = true;
    if (xa != 0 && move2(level, xa, 0))
      stopped = false;
    if (ya != 0 && move2(level, 0, ya))
      stopped = false;
    if (!stopped) {
      int xt = x >> 4;
      int yt = y >> 4;
      Tile::tiles[level.getTile(xt, yt)]->steppedOn(level, xt, yt, *this);
    }
    return !stopped;
  }
  return true;
}

bool Entity::move2(Level& level, int xa, int ya)
{
  sassert(xa != 0 || ya != 0, "Entity.move2() should be called if you're moving");
  sassert((xa != 0) != (ya != 0), "Entity.move2() can only move along one axis at a time");

  int xto0 = ((x)-xr) >> 4;
  int yto0 = ((y)-yr) >> 4;
  int xto1 = ((x)+xr) >> 4;
  int yto1 = ((y)+yr) >> 4;

  int xt0 = ((x + xa) - xr) >> 4;
  int yt0 = ((y + ya) - yr) >> 4;
  int xt1 = ((x + xa) + xr) >> 4;
  int yt1 = ((y + ya) + yr) >> 4;
  bool blocked = false;

  for (int yt = yt0; yt <= yt1; yt++)
    for (int xt = xt0; xt <= xt1; xt++) {
      if (xt >= xto0 && xt <= xto1 && yt >= yto0 && yt <= yto1)
        continue;

      Tile* tile = Tile::tiles[level.getTile(xt, yt)];

      tile->bumpedInto(level, xt, yt, *this);

      if (!tile->mayPass(level, xt, yt, *this)) {
        blocked = true;
        return false;
      }
    }
  if (blocked)
    return false;

  auto wasInside = level.getEntities(x - xr, y - yr, x + xr, y + yr);
  auto isInside = level.getEntities(x + xa - xr, y + ya - yr, x + xa + xr, y + ya + yr);

  for (auto e : isInside) {
    if (e.get() == this)
      continue;

    e->touchedBy(level, *this);
  }

  auto wasInsideBegin = wasInside.begin();
  auto wasInsideEnd = wasInside.end();

  for (auto e : isInside) {
    if (e.get() == this)
      continue;

    if (std::find(wasInsideBegin, wasInsideEnd, e) == wasInsideEnd) {
      continue;
    }

    if (e->blocks(*this)) {
      return false;
    }
  }

  x += xa;
  y += ya;
  return true;
}

bool Entity::interact(Player& player, Item& item, int attackDir)
{
  return item.interact(player, *this, attackDir);
}

void Entity::serializeData(std::ostream& s) {

  nbt::begin_named_compound(s, "Entity");
  nbt::write_named_int(s, "x", x);
  nbt::write_named_int(s, "y", y);
  nbt::write_named_int(s, "xr", xr);
  nbt::write_named_int(s, "yr", yr);
  nbt::write_named_byte(s, "removed", removed);
  nbt::close_compound(s);
}

void Entity::deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) {
  if (name == "Entity") {
    nbt::read_tagged_compound(s, tag, [this, &s](nbt::Tag tag, std::string name) {
      if (name == "x") {
        x = nbt::read_tagged_number<int>(s, tag);
      } else if (name == "y") {
        y = nbt::read_tagged_number<int>(s, tag);
      } else if (name == "xr") {
        xr = nbt::read_tagged_number<int>(s, tag);
      } else if (name == "yr") {
        yr = nbt::read_tagged_number<int>(s, tag);
      } else if (name == "removed") {
        removed = nbt::read_tagged_number<bool>(s, tag);
      }
    });
  } else {
    nbt::Serializable::deserializeDataProperty(s, tag, name);
  }
}

typedef std::function<std::unique_ptr<Entity>()> BlankConstructor;
static std::unordered_map<std::string_view, BlankConstructor> deserializeMap = {
  { "ItemEntity", []() { return std::make_unique<ItemEntity>(std::make_shared<PowerGloveItem>(),0,0); } },
  { "Player", []() { return std::make_unique<Player>(); } },
  { "AirWizard", []() { return std::make_unique<AirWizard>(); } },
  { "Slime", []() { return std::make_unique<Slime>(0); } },
  { "Zombie", []() { return std::make_unique<Zombie>(0); } },
  { "Lantern", []() { return std::make_unique<Lantern>(); } },
  { "Spark", []() { return std::make_unique<Spark>(); } },
  { "Chest", []() { return std::make_unique<Chest>(); } },
  { "Anvil", []() { return std::make_unique<Anvil>(); } },
  { "Furnace", []() { return std::make_unique<Furnace>(); } },
  { "Oven", []() { return std::make_unique<Oven>(); } },
  { "Workbench", []() { return std::make_unique<Workbench>(); } },
};

// expects to be reading from a compound payload
std::unique_ptr<Entity> Entity::deserialize(std::istream& s, nbt::Tag tag)
{
  return nbt::Serializable::deserialize(s, tag, deserializeMap);
}
