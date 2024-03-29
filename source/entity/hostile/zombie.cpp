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
  int yt = 7;

  int flip = (walkDist >> 3) & 1;

  if (dir == 1) {
    xt += 1;
  } else if (dir > 1) {
    const int SIDE_ANIMATION[4] = { 0, 1, 0, 2 };

    xt += 2 + SIDE_ANIMATION[(walkDist >> 3) % 4];
    flip = dir == 2;
  }

  int xo = x - 8;
  int yo = y - 11;
  auto colors = Color::getArray<8>({ -1, 10, 252, 050 });

  if (lvl == 2)
    colors = Color::getArray<8>({ -1, 100, 522, 050 });
  if (lvl == 3)
    colors = Color::getArray<8>({ -1, 111, 444, 050 });
  if (lvl == 4)
    colors = Color::getArray<8>({ -1, 000, 111, 020 });

  if (hurtTime > 0) {
    colors = Color::getArray<8>({ -1, 555, 555, 555 });
  }

  screen.renderTile(xo, yo, xt + yt * 16, colors, flip);
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

void Zombie::serializeData(std::ostream& s) {
  Mob::serializeData(s);
  nbt::write_named_int(s, "xa", xa);
  nbt::write_named_int(s, "ya", ya);
  nbt::write_named_int(s, "lvl", lvl);
  nbt::write_named_int(s, "randomWalkTime", randomWalkTime);
}

void Zombie::deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) {
  if (name == "xa") {
    xa = nbt::read_tagged_number<int>(s, tag);
  } else if (name == "ya") {
    ya = nbt::read_tagged_number<int>(s, tag);
  } else if (name == "lvl") {
    lvl = nbt::read_tagged_number<int>(s, tag);
  } else if (name == "randomWalkTime") {
    randomWalkTime = nbt::read_tagged_number<int>(s, tag);
  } else {
    Mob::deserializeDataProperty(s, tag, name);
  }
}