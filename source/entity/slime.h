#pragma once

#include "mob.h"

class Slime : public Mob
{
private:
  int xa = 0;
  int ya = 0;
  int jumpTime = 0;
  int lvl;

public:
  Slime(int lvl);

  void tick(Game &game, Level &level, std::shared_ptr<Entity> self) override;
  void render(Screen &screen) override;
  void touchedBy(Level &level, Entity &entity) override;

protected:
  void die(Game &game, Level &level) override;
};
