#include "resource.h"

#include "../../gfx/color.h"
#include "plantableresource.h"
#include "foodresource.h"

Resource Resource::wood = Resource("Wood", 1 + 4 * 32, Color::get(-1, 200, 531, 430));
Resource Resource::stone = Resource("Stone", 2 + 4 * 32, Color::get(-1, 111, 333, 555));
Resource Resource::flower = PlantableResource("Flower", 0 + 4 * 32, Color::get(-1, 10, 444, 330), Tile::flower->id, {Tile::grass->id});
Resource Resource::acorn = PlantableResource("Acorn", 3 + 4 * 32, Color::get(-1, 100, 531, 320), Tile::treeSapling->id, {Tile::grass->id});
Resource Resource::dirt = PlantableResource("Dirt", 2 + 4 * 32, Color::get(-1, 100, 322, 432), Tile::dirt->id, {Tile::hole->id, Tile::water->id, Tile::lava->id});
Resource Resource::sand = PlantableResource("Sand", 2 + 4 * 32, Color::get(-1, 110, 440, 550), Tile::sand->id, {Tile::grass->id, Tile::dirt->id});
Resource Resource::cactusFlower = PlantableResource("Cactus", 4 + 4 * 32, Color::get(-1, 10, 40, 50), Tile::cactusSapling->id, {Tile::sand->id});
Resource Resource::seeds = PlantableResource("Seeds", 5 + 4 * 32, Color::get(-1, 10, 40, 50), Tile::wheat->id, {Tile::farmland->id});
Resource Resource::wheat = Resource("Wheat", 6 + 4 * 32, Color::get(-1, 110, 330, 550));
Resource Resource::bread = FoodResource("Bread", 8 + 4 * 32, Color::get(-1, 110, 330, 550), 2, 5);
Resource Resource::apple = FoodResource("Apple", 9 + 4 * 32, Color::get(-1, 100, 300, 500), 1, 5);

Resource Resource::coal = Resource("COAL", 10 + 4 * 32, Color::get(-1, 000, 111, 111));
Resource Resource::ironOre = Resource("I.ORE", 10 + 4 * 32, Color::get(-1, 100, 322, 544));
Resource Resource::goldOre = Resource("G.ORE", 10 + 4 * 32, Color::get(-1, 110, 440, 553));
Resource Resource::ironIngot = Resource("IRON", 11 + 4 * 32, Color::get(-1, 100, 322, 544));
Resource Resource::goldIngot = Resource("GOLD", 11 + 4 * 32, Color::get(-1, 110, 330, 553));

Resource Resource::slime = Resource("SLIME", 10 + 4 * 32, Color::get(-1, 10, 30, 50));
Resource Resource::glass = Resource("glass", 12 + 4 * 32, Color::get(-1, 555, 555, 555));
Resource Resource::cloth = Resource("cloth", 1 + 4 * 32, Color::get(-1, 25, 252, 141));
Resource Resource::cloud = PlantableResource("cloud", 2 + 4 * 32, Color::get(-1, 222, 555, 444), Tile::cloud->id, {Tile::infiniteFall->id});
Resource Resource::gem = Resource("gem", 13 + 4 * 32, Color::get(-1, 101, 404, 545));

Resource::Resource(std::string name, int sprite, int color)
    : name(name), sprite(sprite), color(color)
{
  sassert(name.length() <= 6, "Name must be six characters or less");
}
