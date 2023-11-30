#pragma once

#include "entity.h"

class Mob : public Entity
{
protected:
  int walkDist = 0;
  int xKnockback = 0;
  int yKnockback = 0;

  virtual void doHurt(Level& level, int damage, int attackDir);
  virtual void die(Game& game, Level& level);
  virtual bool isSwimming(Level& level);

public:
  int hurtTime = 0;
  int maxHealth = 10;
  int health = maxHealth;
  int swimTimer = 0;
  int tickTime = 0;
  int dir = 0;

  Mob();

  void tick(Game& game, Level& level, std::shared_ptr<Entity> self) override;
  bool move(Level&, int xa, int ya) override;
  bool blocks(Entity& e) override;
  void hurt(Level& level, Tile& tile, int x, int y, int damage) override;
  void hurt(Level& level, Mob& mob, int damage, int attackDir);
  void heal(Level& level, int heal);
  bool findStartPos(Level& level);
  bool canDespawn(Level& level) override;
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;
};
