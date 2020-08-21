#include "mob.h"

#include "../sound/sound.h"
#include "../level/level.h"
#include "particle/textparticle.h"

Mob::Mob()
    : Entity()
{
  x = y = 8;
  xr = 4;
  yr = 3;
}

void Mob::tick(Game &game, Level &level, std::shared_ptr<Entity> self)
{
  tickTime++;
  if (level.getTile(x >> 4, y >> 4) == Tile::lava)
  {
    hurt(level, *this, 4, dir ^ 1);
  }

  if (health <= 0)
  {
    die(game, level);
  }
  if (hurtTime > 0)
    hurtTime--;
}

void Mob::die(Game &game, Level &level)
{
  remove();
}

bool Mob::move(Level &level, int xa, int ya)
{
  if (isSwimming(level))
  {
    if (swimTimer++ % 2 == 0)
      return true;
  }
  if (xKnockback < 0)
  {
    move2(level, -1, 0);
    xKnockback++;
  }
  if (xKnockback > 0)
  {
    move2(level, 1, 0);
    xKnockback--;
  }
  if (yKnockback < 0)
  {
    move2(level, 0, -1);
    yKnockback++;
  }
  if (yKnockback > 0)
  {
    move2(level, 0, 1);
    yKnockback--;
  }
  if (hurtTime > 0)
    return true;
  if (xa != 0 || ya != 0)
  {
    walkDist++;
    if (xa < 0)
      dir = 2;
    if (xa > 0)
      dir = 3;
    if (ya < 0)
      dir = 1;
    if (ya > 0)
      dir = 0;
  }
  return Entity::move(level, xa, ya);
}

bool Mob::isSwimming(Level &level)
{
  auto tile = level.getTile(x >> 4, y >> 4);
  return tile == Tile::water || tile == Tile::lava;
}

bool Mob::blocks(Entity &e)
{
  return e.isBlockableBy(*this);
}

void Mob::hurt(Level &level, Tile &tile, int x, int y, int damage)
{
  int attackDir = dir ^ 1;
  doHurt(level, damage, attackDir);
}

void Mob::hurt(Level &level, Mob &mob, int damage, int attackDir)
{
  doHurt(level, damage, attackDir);
}

void Mob::heal(Level &level, int heal)
{
  if (hurtTime > 0)
    return;

  level.add(std::make_shared<TextParticle>(std::to_string(heal), x, y, Color::get(-1, 50, 50, 50)));
  health += heal;
  if (health > maxHealth)
    health = maxHealth;
}

void Mob::doHurt(Level &level, int damage, int attackDir)
{
  if (hurtTime > 0)
    return;

  if (level.player != NULL)
  {
    int xd = level.player->x - x;
    int yd = level.player->y - y;
    if (xd * xd + yd * yd < 80 * 80)
    {
      Sound::monsterHurt.play();
    }
  }

  level.add(std::make_shared<TextParticle>(std::to_string(damage), x, y, Color::get(-1, 500, 500, 500)));
  health -= damage;
  if (attackDir == 0)
    yKnockback = +6;
  if (attackDir == 1)
    yKnockback = -6;
  if (attackDir == 2)
    xKnockback = -6;
  if (attackDir == 3)
    xKnockback = +6;
  hurtTime = 10;
}

bool Mob::findStartPos(Level &level)
{
  int x = random.nextInt(level.w);
  int y = random.nextInt(level.h);
  int xx = x * 16 + 8;
  int yy = y * 16 + 8;

  if (level.player != NULL)
  {
    int xd = level.player->x - xx;
    int yd = level.player->y - yy;
    if (xd * xd + yd * yd < 144 * 144)
      return false;
  }

  int r = level.monsterDensity * 16;
  if (level.getEntities(xx - r, yy - r, xx + r, yy + r).size() > 0)
    return false;

  if (Tile::tiles[level.getTile(x, y)]->mayPass(level, x, y, *this))
  {
    this->x = xx;
    this->y = yy;
    return true;
  }

  return false;
}
