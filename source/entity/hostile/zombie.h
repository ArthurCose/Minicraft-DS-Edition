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

  void tick(Game& game, Level& level, std::shared_ptr<Entity> self) override;
  void render(Screen& screen) override;
  void touchedBy(Level& level, Entity& entity);
  std::string_view serializedClassName() override { return "Zombie"; }
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;

protected:
  void die(Game& game, Level& level);
};
