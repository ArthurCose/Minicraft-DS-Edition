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

Tile* Tile::tiles[256];

Tile::Tile(unsigned char id) : id(id)
{
  sassert(tiles[id] == NULL, "No duplicate tile ids");
  tiles[id] = this;
}
