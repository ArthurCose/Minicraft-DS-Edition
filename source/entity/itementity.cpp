#include "itementity.h"

ItemEntity::ItemEntity(std::shared_ptr<Item> item, int x, int y)
{
  this->item = item;
  xx = this->x = x;
  yy = this->y = y;
  xr = 3;
  yr = 3;

  zz = 2;
  xa = random.nextGaussian() * 0.3;
  ya = random.nextGaussian() * 0.2;
  za = random.nextFloat() * 0.7 + 1;

  lifeTime = 60 * 10 + random.nextInt(60);
}

void ItemEntity::tick(Game& game, Level& level, std::shared_ptr<Entity> self)
{
  time++;
  if (time >= lifeTime) {
    remove();
    return;
  }
  xx += xa;
  yy += ya;
  zz += za;
  if (zz < 0) {
    zz = 0;
    za *= -0.5;
    xa *= 0.6;
    ya *= 0.6;
  }
  za -= 0.15;
  int ox = x;
  int oy = y;
  int nx = (int)xx;
  int ny = (int)yy;
  int expectedx = nx - x;
  int expectedy = ny - y;
  move(level, nx - x, ny - y);
  int gotx = x - ox;
  int goty = y - oy;
  xx += gotx - expectedx;
  yy += goty - expectedy;

  if (hurtTime > 0)
    hurtTime--;
}

bool ItemEntity::isBlockableBy(Mob& mob)
{
  return false;
}

void ItemEntity::render(Screen& screen)
{
  if (time >= lifeTime - 6 * 20) {
    if (time / 6 % 2 == 0)
      return;
  }
  screen.renderTile(x - 4, y - 4, item->getSprite(), Color::get(-1, 0, 0, 0), 0);
  screen.renderTile(x - 4, y - 4 - (int)(zz), item->getSprite(), item->getColor(), 0);
}

void ItemEntity::take(Player& player)
{
  Sound::pickup.play();
  player.score++;
  remove();
}

void ItemEntity::touchedBy(Level& level, Entity& entity)
{
  if (time > 30)
    entity.touchItem(*this);
}