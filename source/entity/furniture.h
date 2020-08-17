#pragma once

#include "entity.h"
#include "player.h"

class Furniture : public Entity
{
public:
  int col, sprite;
  std::string name;

  Furniture(std::string name);
  Furniture(Furniture &f);

  void tick(Game &game, Level &level, std::shared_ptr<Entity> self) override;

  void render(Screen &screen) override;
  bool blocks(Entity &e) override;
  void take(Player &player);

protected:
  void touchedBy(Level &level, Entity &entity) override;

private:
  int pushTime = 0;
  int pushDir = -1;
};
