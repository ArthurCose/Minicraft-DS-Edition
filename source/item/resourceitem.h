#pragma once

#include <string>
#include "../gfx/color.h"
#include "item.h"
#include "resource/resource.h"

class ResourceItem : public Item
{
public:
  const Resource *resource;
  int count = 1;

  ResourceItem(const Resource *resource);
  ResourceItem(const Resource *resource, int count);

  int getColor() override;
  int getSprite() override;
  void renderIcon(Screen &screen, int x, int y) override;
  void renderInventory(Screen &screen, int x, int y) override;
  std::string getName() override;
  bool interactOn(Tile &tile, Level &level, int xt, int yt, Player &player, int attackDir) override;
  bool isDepleted();
};
