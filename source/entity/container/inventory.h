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
  bool hasResources(const Resource *r, int count);
  bool removeResource(const Resource *r, int count);
  bool removeItem(Item &item);
  int count(const Item &item);

private:
  std::shared_ptr<ResourceItem> findResource(const Resource *resource);
};
