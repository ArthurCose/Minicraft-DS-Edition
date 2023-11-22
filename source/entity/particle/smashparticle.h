#pragma once

#include "../../game.h"
#include "../../gfx/screen.h"
#include "../../level/level.h"
#include "../entity.h"

class SmashParticle : public Entity
{
private:
  int time = 0;

public:
  SmashParticle(int x, int y);

  void tick(Game& game, Level& level, std::shared_ptr<Entity> self) override;
  void render(Screen& screen) override;
};
