#include "smashparticle.h"

#include "../../gfx/color.h"
#include "../../sound/sound.h"

SmashParticle::SmashParticle(int x, int y)
{
  this->x = x;
  this->y = y;
  Sound::monsterHurt.play();
}

void SmashParticle::tick(Game &game, Level &level, std::shared_ptr<Entity> self)
{
  time++;
  if (time > 10)
  {
    remove();
  }
}

void SmashParticle::render(Screen &screen)
{
  int col = Color::get(-1, 555, 555, 555);
  screen.renderTile(x - 8, y - 8, 5 + 12 * 32, col, 2);
  screen.renderTile(x - 0, y - 8, 5 + 12 * 32, col, 3);
  screen.renderTile(x - 8, y - 0, 5 + 12 * 32, col, 0);
  screen.renderTile(x - 0, y - 0, 5 + 12 * 32, col, 1);
}
