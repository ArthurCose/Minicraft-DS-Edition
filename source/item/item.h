#pragma once

#include <memory>
#include "../menu/listitem.h"

class Screen;
class Tile;
class Level;
class Entity;
class Player;
class ItemEntity;

class Item : public ListItem
{
public:
  virtual int getColor() { return 0; }

  virtual int getSprite() { return 0; }

  virtual void onTake(ItemEntity &itemEntity) {}

  virtual bool interact(Player &player, Entity &entity, int attackDir) { return false; }

  virtual void renderIcon(Screen &screen, int x, int y) {}

  virtual bool interactOn(Tile &tile, Level &level, int xt, int yt, Player &player, int attackDir) { return false; }

  virtual bool isDepleted() { return false; }

  virtual bool canAttack() { return false; }

  virtual int getAttackDamageBonus(Entity &e) { return 0; }

  virtual std::string getName() { return "ERROR"; }

  virtual bool matches(Item &item) { return item.getName() == getName(); }

  virtual std::shared_ptr<Item> clone() { return std::make_shared<Item>(); }
};
