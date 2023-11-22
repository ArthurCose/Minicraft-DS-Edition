#include "resourceitem.h"

ResourceItem::ResourceItem(Resource::ID resourceId)
  : resourceId(resourceId)
{
}

ResourceItem::ResourceItem(Resource::ID resourceId, int count)
  : resourceId(resourceId)
{
  this->count = count;
}

std::string ResourceItem::getName() const
{
  return Resource::resources[resourceId]->name;
}

int ResourceItem::getColor() const
{
  return Resource::resources[resourceId]->color;
}

int ResourceItem::getSprite() const
{
  return Resource::resources[resourceId]->sprite;
}

void ResourceItem::renderIcon(Screen& screen, int x, int y)
{
  auto resource = Resource::resources[resourceId];

  screen.renderTile(x, y, resource->sprite, resource->color, 0);
}

void ResourceItem::renderInventory(Screen& screen, int x, int y)
{
  auto resource = Resource::resources[resourceId];

  screen.renderTile(x, y, resource->sprite, resource->color, 0);
  screen.renderText(resource->name, x + 32, y, Color::get(-1, 555, 555, 555));

  int cc = count;
  if (cc > 999)
    cc = 999;

  screen.renderText(std::to_string(cc), x + 8, y, Color::get(-1, 444, 444, 444));
}

bool ResourceItem::interactOn(Tile& tile, Level& level, int xt, int yt, Player& player, int attackDir)
{
  auto resource = Resource::resources[resourceId];

  if (resource->interactOn(tile, level, xt, yt, player, attackDir)) {
    count--;
    return true;
  }
  return false;
}

bool ResourceItem::isDepleted() { return count <= 0; }

std::shared_ptr<Item> ResourceItem::clone()
{
  return std::make_shared<ResourceItem>(resourceId);
}
