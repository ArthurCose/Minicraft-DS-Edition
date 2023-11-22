#include "textparticle.h"

TextParticle::TextParticle(std::string msg, int x, int y, int col)
{
  this->msg = msg;
  this->x = x;
  this->y = y;
  this->col = col;
  xx = x;
  yy = y;
  zz = 2;
  xa = random.nextGaussian() * 0.3;
  ya = random.nextGaussian() * 0.2;
  za = random.nextFloat() * 0.7 + 2;
}

void TextParticle::tick(Game& game, Level& level, std::shared_ptr<Entity> self)
{
  time++;
  if (time > 60) {
    remove();
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
  x = (int)xx;
  y = (int)yy;
}

void TextParticle::render(Screen& screen)
{
  screen.renderText(msg, x - msg.length() * 4 + 1, y - (int)(zz)+1, Color::get(-1, 0, 0, 0));
  screen.renderText(msg, x - msg.length() * 4, y - (int)(zz), col);
}
