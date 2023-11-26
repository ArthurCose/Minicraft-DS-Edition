#pragma once

#include "../entity.h"
#include "../../gfx/color.h"
#include "../../gfx/screen.h"
#include "../../fixedpoint.h"

class TextParticle : public Entity
{
private:
  std::string msg;
  int col;
  int time = 0;

public:
  fixed32 xa, ya, za;
  fixed32 xx, yy, zz;

  TextParticle(std::string msg, int x, int y, int col);

  void tick(Game& game, Level& level, std::shared_ptr<Entity> self) override;
  void render(Screen& screen) override;
};
