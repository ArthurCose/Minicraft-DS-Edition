#pragma once

#include "../mob.h"

class AirWizard : public Mob
{
private:
  int xa, ya;
  int randomWalkTime = 0;
  int attackDelay = 0;
  int attackTime = 0;
  int attackType = 0;

public:
  AirWizard();

  void tick(Game &game, Level &level, std::shared_ptr<Entity> self) override;
  void render(Screen &screen) override;

protected:
  void touchedBy(Level &level, Entity &entity) override;
  void doHurt(Level &level, int damage, int attackDir) override;
  void die(Game &game, Level &level) override;
};