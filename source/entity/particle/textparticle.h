#pragma once

#include "../entity.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"

class TextParticle : public Entity
{
private:
  std::string msg;
  int col;
  int time = 0;

public:
  double xa, ya, za;
  double xx, yy, zz;

  TextParticle(std::string msg, int x, int y, int col);

  void tick(Game &game, Level &level, std::shared_ptr<Entity> self) override;
  void render(Screen &screen) override;
};
