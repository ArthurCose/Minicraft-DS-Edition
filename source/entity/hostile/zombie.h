#pragma once

#include "../mob.h"

class Zombie : public Mob
{
private:
  int xa = 0;
  int ya = 0;
  int lvl;
  int randomWalkTime = 0;

public:
  Zombie(int lvl);

  void tick(Game &game, Level &level, std::shared_ptr<Entity> self) override;
  void render(Screen &screen) override;
  void touchedBy(Level &level, Entity &entity);

protected:
  void die(Game &game, Level &level);
};
