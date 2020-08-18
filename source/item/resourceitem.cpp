#include "resourceitem.h"

ResourceItem::ResourceItem(const Resource *resource)
    : resource(resource)
{
}

std::string ResourceItem::getName() const
{
  return resource->name;
}

ResourceItem::ResourceItem(const Resource *resource, int count)
    : resource(resource)
{
  this->count = count;
}

int ResourceItem::getColor() const
{
  return resource->color;
}

int ResourceItem::getSprite() const
{
  return resource->sprite;
}

void ResourceItem::renderIcon(Screen &screen, int x, int y)
{
  screen.renderTile(x, y, resource->sprite, resource->color, 0);
}

void ResourceItem::renderInventory(Screen &screen, int x, int y)
{
  screen.renderTile(x, y, resource->sprite, resource->color, 0);
  screen.renderText(resource->name, x + 32, y, Color::get(-1, 555, 555, 555));

  int cc = count;
  if (cc > 999)
    cc = 999;

  screen.renderText(std::to_string(cc), x + 8, y, Color::get(-1, 444, 444, 444));
}

bool ResourceItem::interactOn(Tile &tile, Level &level, int xt, int yt, Player &player, int attackDir)
{
  if (resource->interactOn(tile, level, xt, yt, player, attackDir))
  {
    count--;
    return true;
  }
  return false;
}

bool ResourceItem::isDepleted() { return count <= 0; }
