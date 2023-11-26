#pragma once

#include "entity.h"
#include "../gfx/screen.h"
#include "../fixedpoint.h"

class ItemEntity : public Entity
{
private:
  int lifeTime;
  int time = 0;

protected:
  int walkDist = 0;
  int dir = 0;
  int xKnockback, yKnockback;
  void touchedBy(Level& level, Entity& entity) override;

public:
  int hurtTime = 0;
  fixed32 xa{}, ya{}, za{};
  fixed32 xx{}, yy{}, zz{};
  std::shared_ptr<Item> item;

  ItemEntity(std::shared_ptr<Item> item, int x, int y);

  void tick(Game& game, Level& level, std::shared_ptr<Entity> self) override;
  bool isBlockableBy(Mob& mob) override;
  void render(Screen& screen) override;
  void take(Player& player);
  std::string_view serializedClassName() override { return "ItemEntity"; }
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;
};