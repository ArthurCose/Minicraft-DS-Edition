#pragma once

#include <memory>
#include "../entity.h"
#include "../../fixedpoint.h"

class Spark : public Entity
{
private:
  int lifeTime{};
  int time{};
  int ownerId = -1;

public:
  fixed32 xa{}, ya{};
  fixed32 xx{}, yy{};

  Spark() {}
  Spark(std::shared_ptr<Entity> owner, fixed32 xa, fixed32 ya);

  void tick(Game& game, Level& level, std::shared_ptr<Entity> self) override;
  void render(Screen& screen) override;
  bool isBlockableBy(Mob& mob) override;
  std::string_view serializedClassName() override { return "Spark"; }
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;
};
