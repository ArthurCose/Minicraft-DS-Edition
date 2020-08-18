#include "spark.h"

#include <memory>
#include "../../level/level.h"
#include "../../gfx/screen.h"
#include "../../gfx/color.h"

Spark::Spark(std::shared_ptr<Entity> owner, double xa, double ya)
    : owner_weak(owner)
{

  xx = this->x = owner->x;
  yy = this->y = owner->y;
  xr = 0;
  yr = 0;

  this->xa = xa;
  this->ya = ya;

  lifeTime = 60 * 10 + random.nextInt(30);
}

void Spark::tick(Game &game, Level &level, std::shared_ptr<Entity> self)
{
  auto owner = std::dynamic_pointer_cast<Mob>(owner_weak.lock());

  if (!owner)
  {
    remove();
    return;
  }

  time++;
  if (time >= lifeTime)
  {
    remove();
    return;
  }
  xx += xa;
  yy += ya;
  x = (int)xx;
  y = (int)yy;

  auto toHit = level.getEntities(x, y, x, y);

  for (auto e : toHit)
  {
    if (auto mob = std::dynamic_pointer_cast<Mob>(e))
    {
      e->hurt(level, *owner, 1, mob->dir ^ 1);
    }
  }
}

void Spark::render(Screen &screen)
{
  if (time >= lifeTime - 6 * 20)
  {
    if (time / 6 % 2 == 0)
      return;
  }

  int xt = 8;
  int yt = 13;

  screen.renderTile(x - 4, y - 4 - 2, xt + yt * 32, Color::get(-1, 555, 555, 555), random.nextInt(4));
  screen.renderTile(x - 4, y - 4 + 2, xt + yt * 32, Color::get(-1, 000, 000, 000), random.nextInt(4));
}

bool Spark::isBlockableBy(Mob &mob)
{
  return false;
}
