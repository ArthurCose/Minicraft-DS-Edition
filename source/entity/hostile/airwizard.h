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

  bool canDespawn(Level& level) override { return false; }
  void tick(Game& game, Level& level, std::shared_ptr<Entity> self) override;
  void render(Screen& screen) override;

  std::string_view serializedClassName() override { return "AirWizard"; }
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;

protected:
  void touchedBy(Level& level, Entity& entity) override;
  void doHurt(Level& level, int damage, int attackDir) override;
  void die(Game& game, Level& level) override;
};