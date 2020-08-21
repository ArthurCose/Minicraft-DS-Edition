#include "tile.h"

#include <nds.h>
#include <ctime>
#include "grasstile.h"
#include "rocktile.h"
#include "watertile.h"
#include "flowertile.h"
#include "treetile.h"
#include "dirttile.h"
#include "sandtile.h"
#include "cactustile.h"
#include "holetile.h"
#include "saplingtile.h"
#include "farmtile.h"
#include "wheattile.h"
#include "lavatile.h"
#include "stairstile.h"
#include "infinitefalltile.h"
#include "cloudtile.h"
#include "hardrocktile.h"
#include "oretile.h"
#include "cloudcactustile.h"
#include "../../gfx/screen.h"
#include "../../item/resource/resource.h"

Random Tile::random(time(NULL));

int Tile::tickCount = 0;

Tile *Tile::tiles[256];
Tile *Tile::grass = new GrassTile(0);
Tile *Tile::rock = new RockTile(1);
Tile *Tile::water = new WaterTile(2);
Tile *Tile::flower = new FlowerTile(3);
Tile *Tile::tree = new TreeTile(4);
Tile *Tile::dirt = new DirtTile(5);
Tile *Tile::sand = new SandTile(6);
Tile *Tile::cactus = new CactusTile(7);
Tile *Tile::hole = new HoleTile(8);
Tile *Tile::treeSapling = new SaplingTile(9, *grass, *tree);
Tile *Tile::cactusSapling = new SaplingTile(10, *sand, *cactus);
Tile *Tile::farmland = new FarmTile(11);
Tile *Tile::wheat = new WheatTile(12);
Tile *Tile::lava = new LavaTile(13);
Tile *Tile::stairsDown = new StairsTile(14, false);
Tile *Tile::stairsUp = new StairsTile(15, true);
Tile *Tile::infiniteFall = new InfiniteFallTile(16);
Tile *Tile::cloud = new CloudTile(17);
Tile *Tile::hardRock = new HardRockTile(18);
Tile *Tile::ironOre = new OreTile(19, Resource::ironOre);
Tile *Tile::goldOre = new OreTile(20, Resource::goldOre);
Tile *Tile::gemOre = new OreTile(21, Resource::gem);
Tile *Tile::cloudCactus = new CloudCactusTile(22);

Tile::Tile(unsigned char id)
    : id(id)
{
  sassert(tiles[id] == NULL, "No duplicate tile ids");
  tiles[id] = this;
}
