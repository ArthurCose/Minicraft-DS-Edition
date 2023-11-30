#pragma once

#include <memory>
#include "entity.h"
#include "player.h"

class Furniture : public Entity
{
public:
  int col, sprite;
  std::string name;

  Furniture(std::string name);

  void tick(Game& game, Level& level, std::shared_ptr<Entity> self) override;

  void render(Screen& screen) override;
  bool blocks(Entity& e) override;
  void take(Player& player);
  virtual std::shared_ptr<Furniture> clone();
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;

protected:
  bool shouldTake = false;
  void touchedBy(Level& level, Entity& entity) override;

private:
  int pushTime = 0;
  int pushDir = -1;
};
