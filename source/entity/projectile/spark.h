#pragma once

#include <memory>
#include "../entity.h"

class Spark : public Entity
{
private:
  int lifeTime;
  int time;
  std::weak_ptr<Entity> owner_weak;

public:
  double xa, ya;
  double xx, yy;

  Spark(std::shared_ptr<Entity> owner, double xa, double ya);

  void tick(Game &game, Level &level, std::shared_ptr<Entity> self) override;
  void render(Screen &screen) override;
  bool isBlockableBy(Mob &mob) override;
};
