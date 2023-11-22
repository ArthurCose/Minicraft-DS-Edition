#include "zombie.h"

#include "../../level/level.h"
#include "../../gfx/screen.h"
#include "../../gfx/color.h"

Zombie::Zombie(int lvl)
{
  this->lvl = lvl;
  x = random.nextInt(64 * 16);
  y = random.nextInt(64 * 16);
  health = maxHealth = lvl * lvl * 10;
}

void Zombie::tick(Game& game, Level& level, std::shared_ptr<Entity> self)
{
  Mob::tick(game, level, self);

  if (level.player != NULL && randomWalkTime == 0) {
    int xd = level.player->x - x;
    int yd = level.player->y - y;
    if (xd * xd + yd * yd < 50 * 50) {
      xa = 0;
      ya = 0;
      if (xd < 0)
        xa = -1;
      if (xd > 0)
        xa = +1;
      if (yd < 0)
        ya = -1;
      if (yd > 0)
        ya = +1;
    }
  }

  int speed = tickTime & 1;
  if (!move(level, xa * speed, ya * speed) || random.nextInt(200) == 0) {
    randomWalkTime = 60;
    xa = (random.nextInt(3) - 1) * random.nextInt(2);
    ya = (random.nextInt(3) - 1) * random.nextInt(2);
  }
  if (randomWalkTime > 0)
    randomWalkTime--;
}

void Zombie::render(Screen& screen)
{
  int xt = 0;
  int yt = 14;

  int flip1 = (walkDist >> 3) & 1;
  int flip2 = (walkDist >> 3) & 1;

  if (dir == 1) {
    xt += 2;
  }
  if (dir > 1) {

    flip1 = 0;
    flip2 = ((walkDist >> 4) & 1);
    if (dir == 2) {
      flip1 = 1;
    }
    xt += 4 + ((walkDist >> 3) & 1) * 2;
  }

  int xo = x - 8;
  int yo = y - 11;

  int col = Color::get(-1, 10, 252, 050);

  if (lvl == 2)
    col = Color::get(-1, 100, 522, 050);
  if (lvl == 3)
    col = Color::get(-1, 111, 444, 050);
  if (lvl == 4)
    col = Color::get(-1, 000, 111, 020);

  if (hurtTime > 0) {
    col = Color::get(-1, 555, 555, 555);
  }

  screen.renderTile(xo + 8 * flip1, yo + 0, xt + yt * 32, col, flip1);
  screen.renderTile(xo + 8 - 8 * flip1, yo + 0, xt + 1 + yt * 32, col, flip1);
  screen.renderTile(xo + 8 * flip2, yo + 8, xt + (yt + 1) * 32, col, flip2);
  screen.renderTile(xo + 8 - 8 * flip2, yo + 8, xt + 1 + (yt + 1) * 32, col, flip2);
}

void Zombie::touchedBy(Level& level, Entity& entity)
{
  if (dynamic_cast<Player*>(&entity)) {
    entity.hurt(level, *this, lvl + 1, dir);
  }
}

void Zombie::die(Game& game, Level& level)
{
  Mob::die(game, level);

  int count = random.nextInt(2) + 1;

  for (int i = 0; i < count; i++) {
    level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::cloth), x + random.nextInt(11) - 5, y + random.nextInt(11) - 5));
  }

  if (level.player != NULL) {
    level.player->score += 50 * lvl;
  }
}
