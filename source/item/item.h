#pragma once

#include <memory>
#include "../menu/listitem.h"
#include "../nbt.h"

class Screen;
class Tile;
class Level;
class Entity;
class Player;
class ItemEntity;

class Item : public ListItem, public nbt::Serializable
{
public:
  virtual std::string_view getName() const { return "ERROR"; }

  virtual int getColor() const { return 0; }

  virtual int getSprite() const { return 0; }

  virtual bool interact(Player& player, Entity& entity, int attackDir) { return false; }

  virtual void renderIcon(Screen& screen, int x, int y) {}

  virtual bool interactOn(Tile& tile, Level& level, int xt, int yt, Player& player, int attackDir) { return false; }

  virtual bool isDepleted() { return false; }

  virtual bool canAttack() { return false; }

  virtual int getAttackDamageBonus(Entity& e) { return 0; }

  virtual bool matches(const Item& item) { return item.getName() == getName(); }

  virtual std::shared_ptr<Item> clone() { return std::make_shared<Item>(); }

  static std::unique_ptr<Item> deserialize(std::istream& s, nbt::Tag tag);
};
