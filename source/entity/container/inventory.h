#pragma once

#include <vector>
#include <memory>
#include "../../item/item.h"
#include "../../item/resource/resource.h"
#include "../../item/resourceitem.h"

class Inventory
{
public:
  std::vector<std::shared_ptr<Item>> items;

  void add(std::shared_ptr<Item> item);
  void add(int slot, std::shared_ptr<Item> item);
  bool hasResources(Resource &r, int count);
  bool removeResource(Resource &r, int count);
  bool removeItem(Item &item);
  int count(Item &item);

private:
  std::shared_ptr<ResourceItem> findResource(Resource &resource);
};
