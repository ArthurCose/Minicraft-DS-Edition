#include "resource.h"

#include "plantableresource.h"
#include "foodresource.h"
#include "../../gfx/color.h"

Resource *Resource::resources[256];

Resource::Resource(std::string name, int sprite, int color)
    : name(name), sprite(sprite), color(color)
{
  sassert(name.length() <= 6, "Name must be six characters or less");
}
