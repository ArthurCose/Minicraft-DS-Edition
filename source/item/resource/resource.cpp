#include "resource.h"

#include "plantableresource.h"
#include "foodresource.h"
#include "../../gfx/color.h"

const Resource *Resource::wood = new Resource("Wood", 1 + 4 * 32, Color::get(-1, 200, 531, 430));
const Resource *Resource::stone = new Resource("Stone", 2 + 4 * 32, Color::get(-1, 111, 333, 555));
const Resource *Resource::flower = new PlantableResource("Flower", 0 + 4 * 32, Color::get(-1, 10, 444, 330), Tile::flower->id, {Tile::grass->id});
const Resource *Resource::acorn = new PlantableResource("Acorn", 3 + 4 * 32, Color::get(-1, 100, 531, 320), Tile::treeSapling->id, {Tile::grass->id});
const Resource *Resource::dirt = new PlantableResource("Dirt", 2 + 4 * 32, Color::get(-1, 100, 322, 432), Tile::dirt->id, {Tile::hole->id, Tile::water->id, Tile::lava->id});
const Resource *Resource::sand = new PlantableResource("Sand", 2 + 4 * 32, Color::get(-1, 110, 440, 550), Tile::sand->id, {Tile::grass->id, Tile::dirt->id});
const Resource *Resource::cactusFlower = new PlantableResource("Cactus", 4 + 4 * 32, Color::get(-1, 10, 40, 50), Tile::cactusSapling->id, {Tile::sand->id});
const Resource *Resource::seeds = new PlantableResource("Seeds", 5 + 4 * 32, Color::get(-1, 10, 40, 50), Tile::wheat->id, {Tile::farmland->id});
const Resource *Resource::wheat = new Resource("Wheat", 6 + 4 * 32, Color::get(-1, 110, 330, 550));
const Resource *Resource::bread = new FoodResource("Bread", 8 + 4 * 32, Color::get(-1, 110, 330, 550), 2, 5);
const Resource *Resource::apple = new FoodResource("Apple", 9 + 4 * 32, Color::get(-1, 100, 300, 500), 1, 5);

const Resource *Resource::coal = new Resource("COAL", 10 + 4 * 32, Color::get(-1, 000, 111, 111));
const Resource *Resource::ironOre = new Resource("I.ORE", 10 + 4 * 32, Color::get(-1, 100, 322, 544));
const Resource *Resource::goldOre = new Resource("G.ORE", 10 + 4 * 32, Color::get(-1, 110, 440, 553));
const Resource *Resource::ironIngot = new Resource("IRON", 11 + 4 * 32, Color::get(-1, 100, 322, 544));
const Resource *Resource::goldIngot = new Resource("GOLD", 11 + 4 * 32, Color::get(-1, 110, 330, 553));

const Resource *Resource::slime = new Resource("SLIME", 10 + 4 * 32, Color::get(-1, 10, 30, 50));
const Resource *Resource::glass = new Resource("glass", 12 + 4 * 32, Color::get(-1, 555, 555, 555));
const Resource *Resource::cloth = new Resource("cloth", 1 + 4 * 32, Color::get(-1, 25, 252, 141));
const Resource *Resource::cloud = new PlantableResource("cloud", 2 + 4 * 32, Color::get(-1, 222, 555, 444), Tile::cloud->id, {Tile::infiniteFall->id});
const Resource *Resource::gem = new Resource("gem", 13 + 4 * 32, Color::get(-1, 101, 404, 545));

Resource::Resource(std::string name, int sprite, int color)
    : name(name), sprite(sprite), color(color)
{
  sassert(name.length() <= 6, "Name must be six characters or less");
}
