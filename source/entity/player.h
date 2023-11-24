#pragma once

#include "../game.h"
#include "../level/level.h"
#include "../item/inventory.h"
#include "itementity.h"
#include "mob.h"

class Player : public Mob
{
public:
  Inventory inventory;
  int stamina = 0;
  int staminaRecharge = 0;
  int staminaRechargeDelay = 0;
  int score = 0;
  int maxStamina = 10;
  int invulnerableTime = 0;

  Player();

  void tick(Game& game, Level& level, std::shared_ptr<Entity> self) override;
  void render(Screen& screen) override;
  void touchItem(ItemEntity& itemEntity) override;
  bool findStartPos(Level& level);
  bool payStamina(int cost);
  bool canSwim() override;
  int getLightRadius() override;
  void setSelectedItemIndex(int index);
  int getSelectedItemIndex();
  bool holdingItem();
  void setItemHeld(bool status);
  std::shared_ptr<Item> getActiveItem();
  std::string_view serializedClassName() override { return "Player"; }
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;

private:
  int attackTime = 0, attackDir = 0;
  int onStairDelay = 0;
  bool swimming = false;
  int selectedItemIndex = 0;
  bool itemHeld = false;

  void updateInventory(Game& game);
  void attack(Level& level);
  bool use(Game& game, Level& level);
  bool use(Game& game, Level& level, int x0, int y0, int x1, int y1);
  bool interact(Level& level, int x0, int y0, int x1, int y1);
  void hurt(Level& level, int x0, int y0, int x1, int y1);
  int getAttackDamage(Entity& e);

protected:
  void die(Game& game, Level& level) override;
  void touchedBy(Level& level, Entity& entity) override;
  void doHurt(Level& level, int damage, int attackDir) override;
};
