#include "spark.h"

#include <memory>
#include "../../level/level.h"
#include "../../gfx/screen.h"
#include "../../gfx/color.h"

Spark::Spark(std::shared_ptr<Entity> owner, fixed32 xa, fixed32 ya)
{
  ownerId = owner->id;
  xx = this->x = owner->x;
  yy = this->y = owner->y;
  xr = 0;
  yr = 0;

  this->xa = xa;
  this->ya = ya;

  lifeTime = 60 * 10 + random.nextInt(30);
}

void Spark::tick(Game& game, Level& level, std::shared_ptr<Entity> self)
{
  auto owner = std::dynamic_pointer_cast<Mob>(Entity::getEntityById(ownerId));

  if (!owner) {
    remove();
    return;
  }

  time++;
  if (time >= lifeTime) {
    remove();
    return;
  }
  xx += xa;
  yy += ya;
  x = (int)xx;
  y = (int)yy;

  auto toHit = level.getEntities(x, y, x, y);

  for (auto e : toHit) {
    if (e.get() == owner.get())
      continue;

    if (auto mob = std::dynamic_pointer_cast<Mob>(e)) {
      e->hurt(level, *owner, 1, mob->dir ^ 1);
    }
  }
}

void Spark::render(Screen& screen)
{
  if (time >= lifeTime - 6 * 20) {
    if (time / 6 % 2 == 0)
      return;
  }

  int r = random.nextInt(16);
  int xt = r >> 1;
  int yt = 9;

  screen.renderTile(x - 8, y - 8, xt + yt * 16, Color::getArray<8>({ 000, 555 }), r & 1);
}

bool Spark::isBlockableBy(Mob& mob)
{
  return false;
}

void Spark::serializeData(std::ostream& s) {
  Entity::serializeData(s);
  nbt::write_named_float(s, "xa", xa);
  nbt::write_named_float(s, "ya", ya);
  nbt::write_named_float(s, "xx", xx);
  nbt::write_named_float(s, "yy", yy);
  nbt::write_named_int(s, "lifeTime", lifeTime);
  nbt::write_named_int(s, "time", time);
  nbt::write_named_int(s, "ownerId", ownerId);
}

void Spark::deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) {
  if (name == "xa") {
    xa = nbt::read_tagged_number<float>(s, tag);
  } else if (name == "ya") {
    ya = nbt::read_tagged_number<float>(s, tag);
  } else if (name == "xx") {
    xx = nbt::read_tagged_number<float>(s, tag);
  } else if (name == "yy") {
    yy = nbt::read_tagged_number<float>(s, tag);
  } else if (name == "lifeTime") {
    lifeTime = nbt::read_tagged_number<int>(s, tag);
  } else if (name == "time") {
    time = nbt::read_tagged_number<int>(s, tag);
  } else if (name == "ownerId") {
    ownerId = nbt::read_tagged_number<int>(s, tag);
  } else {
    Entity::deserializeDataProperty(s, tag, name);
  }
}