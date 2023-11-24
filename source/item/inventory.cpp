#include "inventory.h"

void Inventory::add(std::shared_ptr<Item> item)
{
  add(items.size(), item);
}

void Inventory::add(int slot, std::shared_ptr<Item> item)
{
  auto itemsBegin = items.begin();

  if (auto toTake = std::dynamic_pointer_cast<ResourceItem>(item)) {
    auto has = findResource(toTake->resourceId);

    if (has == NULL) {
      items.insert(itemsBegin + slot, toTake);
    } else {
      has->count += toTake->count;
    }
  } else {
    items.insert(itemsBegin + slot, item);
  }
}

std::shared_ptr<ResourceItem> Inventory::findResource(Resource::ID resource)
{
  for (auto item : items) {
    if (auto resourceItem = std::dynamic_pointer_cast<ResourceItem>(item)) {
      if (resourceItem->resourceId == resource)
        return resourceItem;
    }
  }

  return NULL;
}

bool Inventory::hasResources(Resource::ID resourceId, int count)
{
  auto ri = findResource(resourceId);
  if (ri == NULL)
    return false;
  return ri->count >= count;
}

bool Inventory::removeResource(Resource::ID resourceId, int count)
{
  auto ri = findResource(resourceId);

  if (ri == NULL)
    return false;
  if (ri->count < count)
    return false;

  ri->count -= count;

  if (ri->count <= 0)
    removeItem(*ri);

  return true;
}

bool Inventory::removeItem(Item& item)
{
  for (auto it = items.begin(); it < items.end(); it++) {
    if ((*it)->matches(item)) {
      items.erase(it);
      return true;
    }
  }

  return false;
}

int Inventory::count(const Item& item)
{
  // unsafe hack to avoid exceptions
  // dont pass this pointer
  if (auto resourceItem = dynamic_cast<const ResourceItem*>(&item)) {
    auto ri = findResource(resourceItem->resourceId);

    if (ri != NULL)
      return ri->count;
  } else {
    int count = 0;

    for (auto inventoryItem : items) {
      if (inventoryItem->matches(item))
        count++;
    }

    return count;
  }
  return 0;
}

void Inventory::serialize(std::ostream& s, std::string_view keyName)
{
  nbt::begin_named_list(s, keyName, nbt::Tag::COMPOUND, items.size());

  for (auto& item : items) {
    item->serialize(s);
    nbt::close_compound(s);
  }
}

void Inventory::deserialize(std::istream& s, nbt::Tag tag)
{
  items.clear();

  nbt::read_tagged_list(s, tag, nbt::Tag::COMPOUND, [this, &s](int i) {
    if (std::shared_ptr item = Item::deserialize(s, nbt::Tag::COMPOUND)) {
      items.push_back(item);
    }
  });
}
