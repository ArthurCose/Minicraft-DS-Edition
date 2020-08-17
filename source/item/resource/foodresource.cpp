#include "foodresource.h"

#include "../../entity/player.h"

FoodResource::FoodResource(std::string name, int sprite, int color, int heal, int staminaCost)
    : Resource(name, sprite, color), heal(heal), staminaCost(staminaCost) {}

bool FoodResource::interactOn(Tile &tile, Level &level, int xt, int yt, Player &player, int attackDir)
{
  if (player.health < player.maxHealth && player.payStamina(staminaCost))
  {
    player.heal(level, heal);
    return true;
  }
  return false;
}
