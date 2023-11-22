#pragma once

#include "../shim/random.h"

#include <memory>

class Game;
class Screen;
class Level;
class Tile;
class Player;
class Item;
class ItemEntity;
class Mob;

class Entity
{
protected:
  static Random random;

  bool move2(Level& level, int xa, int ya);

public:
  int x, y;
  int xr = 6;
  int yr = 6;
  bool removed = false;

  bool interact(Player& player, Item& item, int attackDir);
  void remove();
  bool intersects(int x0, int y0, int x1, int y1);

  virtual void tick(Game& game, Level& level, std::shared_ptr<Entity> self) {}
  virtual void render(Screen& screen) {}
  virtual bool move(Level& level, int xa, int ya);
  virtual bool blocks(Entity& e) { return false; }
  virtual void hurt(Level& level, Mob& mob, int dmg, int attackDir) {}
  virtual void hurt(Level& level, Tile& tile, int x, int y, int dmg) {}
  virtual void touchedBy(Level& level, Entity& entity) {};
  virtual bool isBlockableBy(Mob& mob) { return true; }
  virtual void touchItem(ItemEntity& itemEntity) {}
  virtual bool canSwim() { return false; }
  virtual bool use(Game& game, Level& level, Player& player, int attackDir) { return false; }
  virtual int getLightRadius() { return 0; }
};
