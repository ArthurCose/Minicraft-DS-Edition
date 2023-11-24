#pragma once

#include <memory>
#include "../entity.h"

class Spark : public Entity
{
private:
  int lifeTime{};
  int time{};
  int ownerId = -1;

public:
  float xa{}, ya{};
  float xx{}, yy{};

  Spark() {}
  Spark(std::shared_ptr<Entity> owner, float xa, float ya);

  void tick(Game& game, Level& level, std::shared_ptr<Entity> self) override;
  void render(Screen& screen) override;
  bool isBlockableBy(Mob& mob) override;
  std::string_view serializedClassName() override { return "Spark"; }
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;
};
