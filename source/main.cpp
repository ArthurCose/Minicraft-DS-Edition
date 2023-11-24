#include <nds.h>
#include <nds/arm9/background.h>
#include <fat.h>
#include <gl2d.h>
#include "game.h"
#include "icons.h"
#include "item/resource/foodresource.h"
#include "item/resource/plantableresource.h"
#include "level/tile/grasstile.h"
#include "level/tile/rocktile.h"
#include "level/tile/watertile.h"
#include "level/tile/flowertile.h"
#include "level/tile/treetile.h"
#include "level/tile/dirttile.h"
#include "level/tile/sandtile.h"
#include "level/tile/cactustile.h"
#include "level/tile/holetile.h"
#include "level/tile/saplingtile.h"
#include "level/tile/farmtile.h"
#include "level/tile/wheattile.h"
#include "level/tile/lavatile.h"
#include "level/tile/stairstile.h"
#include "level/tile/infinitefalltile.h"
#include "level/tile/cloudtile.h"
#include "level/tile/hardrocktile.h"
#include "level/tile/oretile.h"
#include "level/tile/cloudcactustile.h"

// in ms
static int playtime = 0;
void incrementTime();
void initialize();
void initializePalette();
void initializeMainEngine();
void initializeSubEngine();
void initializeResources();

int main()
{
  initialize();

  Game game;

  int refreshRate = 1000 / 59.8261;
  int lostTime = 0;

  while (true) {
    auto start = playtime;
    game.tick();

    glBegin2D();
    game.render();
    glEnd2D();

    // wait until work is done
    while (dmaBusy(0) || dmaBusy(1) || dmaBusy(2) || dmaBusy(3))
      ;
    dmaCopy(&game.bottomScreen.pixels[0], BG_GFX_SUB, game.bottomScreen.pixels.size());
    glFlush(0);
    auto end = playtime;

    if (game.frameSkipEnabled) {
      auto totalTime = end - start;

      lostTime += std::max(totalTime - refreshRate, 0);

      int skips = 0;

      while (skips < 3 && lostTime > refreshRate) {
        game.tick();
        lostTime -= refreshRate;
        skips++;
      }
    }
  }

  return 0;
}

void initialize()
{
  timerStart(0, ClockDivider_1024, (u16)TIMER_FREQ_1024(1000), incrementTime);

  fatInitDefault();
  vramSetBankA(VRAM_A_TEXTURE);
  vramSetBankB(VRAM_B_TEXTURE); // dedicated to lighting
  vramSetBankC(VRAM_C_SUB_BG);
  vramSetBankD(VRAM_D_TEXTURE); // dedicated to lighting
  vramSetBankF(VRAM_F_TEX_PALETTE);
  keysSetRepeat(20, 6);

  initializePalette();
  initializeMainEngine();
  initializeSubEngine();
  initializeResources();
}

void incrementTime()
{
  playtime++;
}

void initializePalette()
{
  int pp = 0;
  for (int r = 0; r < 6; r++) {
    for (int g = 0; g < 6; g++) {
      for (int b = 0; b < 6; b++) {
        int rr = (r * 255 / 5);
        int gg = (g * 255 / 5);
        int bb = (b * 255 / 5);
        int mid = (rr * 30 + gg * 59 + bb * 11) / 100;

        int r1 = ((rr + mid * 1) / 2) * 230 / 255 + 10;
        int g1 = ((gg + mid * 1) / 2) * 230 / 255 + 10;
        int b1 = ((bb + mid * 1) / 2) * 230 / 255 + 10;
        Screen::palette[pp++] = RGB15(r1 / 8, g1 / 8, b1 / 8);
      }
    }
  }

  Screen::palette[255] = 0;
}

void initializeMainEngine()
{
  videoSetMode(MODE_5_3D);
  glScreen2D();
}

void initializeSubEngine()
{
  videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);

  REG_BG3CNT_SUB = BG_BMP8_256x256;

  // and 1:1 scaling
  REG_BG3PA_SUB = 1 << 8;
  REG_BG3PB_SUB = 0; // BG SCALING X
  REG_BG3PC_SUB = 0; // BG SCALING Y
  REG_BG3PD_SUB = 1 << 8;
  REG_BG3X_SUB = 0;
  REG_BG3Y_SUB = 0;

  dmaCopy(Screen::palette, BG_PALETTE_SUB, 512);
}

// todo: move these variables into non global space?
void initializeResources()
{
  Screen::spriteSheet = std::make_unique<SpriteSheet>(iconsBitmap, 256, 256, 8);

  Tile::tiles[Tile::grass] = new GrassTile(Tile::grass);
  Tile::tiles[Tile::rock] = new RockTile(Tile::rock);
  Tile::tiles[Tile::water] = new WaterTile(Tile::water);
  Tile::tiles[Tile::flower] = new FlowerTile(Tile::flower);
  Tile::tiles[Tile::tree] = new TreeTile(Tile::tree);
  Tile::tiles[Tile::dirt] = new DirtTile(Tile::dirt);
  Tile::tiles[Tile::sand] = new SandTile(Tile::sand);
  Tile::tiles[Tile::cactus] = new CactusTile(Tile::cactus);
  Tile::tiles[Tile::hole] = new HoleTile(Tile::hole);
  Tile::tiles[Tile::treeSapling] = new SaplingTile(Tile::treeSapling, Tile::tiles[Tile::grass], Tile::tiles[Tile::tree]);
  Tile::tiles[Tile::cactusSapling] = new SaplingTile(Tile::cactusSapling, Tile::tiles[Tile::sand], Tile::tiles[Tile::cactus]);
  Tile::tiles[Tile::farmland] = new FarmTile(Tile::farmland);
  Tile::tiles[Tile::wheat] = new WheatTile(Tile::wheat);
  Tile::tiles[Tile::lava] = new LavaTile(Tile::lava);
  Tile::tiles[Tile::stairsDown] = new StairsTile(Tile::stairsDown, false);
  Tile::tiles[Tile::stairsUp] = new StairsTile(Tile::stairsUp, true);
  Tile::tiles[Tile::infiniteFall] = new InfiniteFallTile(Tile::infiniteFall);
  Tile::tiles[Tile::cloud] = new CloudTile(Tile::cloud);
  Tile::tiles[Tile::hardRock] = new HardRockTile(Tile::hardRock);
  Tile::tiles[Tile::ironOre] = new OreTile(Tile::ironOre, Resource::ironOre);
  Tile::tiles[Tile::goldOre] = new OreTile(Tile::goldOre, Resource::goldOre);
  Tile::tiles[Tile::gemOre] = new OreTile(Tile::gemOre, Resource::gem);
  Tile::tiles[Tile::cloudCactus] = new CloudCactusTile(Tile::cloudCactus);

  Resource::resources[Resource::wood] = new Resource("Wood", 1 + 4 * 32, Color::get(-1, 200, 531, 430));
  Resource::resources[Resource::stone] = new Resource("Stone", 2 + 4 * 32, Color::get(-1, 111, 333, 555));
  Resource::resources[Resource::flower] = new PlantableResource("Flower", 0 + 4 * 32, Color::get(-1, 10, 444, 330), Tile::flower, { Tile::grass });
  Resource::resources[Resource::acorn] = new PlantableResource("Acorn", 3 + 4 * 32, Color::get(-1, 100, 531, 320), Tile::treeSapling, { Tile::grass });
  Resource::resources[Resource::dirt] = new PlantableResource("Dirt", 2 + 4 * 32, Color::get(-1, 100, 322, 432), Tile::dirt, { Tile::hole, Tile::water, Tile::lava });
  Resource::resources[Resource::sand] = new PlantableResource("Sand", 2 + 4 * 32, Color::get(-1, 110, 440, 550), Tile::sand, { Tile::grass, Tile::dirt });
  Resource::resources[Resource::cactusFlower] = new PlantableResource("Cactus", 4 + 4 * 32, Color::get(-1, 10, 40, 50), Tile::cactusSapling, { Tile::sand });
  Resource::resources[Resource::seeds] = new PlantableResource("Seeds", 5 + 4 * 32, Color::get(-1, 10, 40, 50), Tile::wheat, { Tile::farmland });
  Resource::resources[Resource::wheat] = new Resource("Wheat", 6 + 4 * 32, Color::get(-1, 110, 330, 550));
  Resource::resources[Resource::bread] = new FoodResource("Bread", 8 + 4 * 32, Color::get(-1, 110, 330, 550), 2, 5);
  Resource::resources[Resource::apple] = new FoodResource("Apple", 9 + 4 * 32, Color::get(-1, 100, 300, 500), 1, 5);
  Resource::resources[Resource::coal] = new Resource("COAL", 10 + 4 * 32, Color::get(-1, 000, 111, 111));
  Resource::resources[Resource::ironOre] = new Resource("I.ORE", 10 + 4 * 32, Color::get(-1, 100, 322, 544));
  Resource::resources[Resource::goldOre] = new Resource("G.ORE", 10 + 4 * 32, Color::get(-1, 110, 440, 553));
  Resource::resources[Resource::ironIngot] = new Resource("IRON", 11 + 4 * 32, Color::get(-1, 100, 322, 544));
  Resource::resources[Resource::goldIngot] = new Resource("GOLD", 11 + 4 * 32, Color::get(-1, 110, 330, 553));
  Resource::resources[Resource::slime] = new Resource("SLIME", 10 + 4 * 32, Color::get(-1, 10, 30, 50));
  Resource::resources[Resource::glass] = new Resource("glass", 12 + 4 * 32, Color::get(-1, 555, 555, 555));
  Resource::resources[Resource::cloth] = new Resource("cloth", 1 + 4 * 32, Color::get(-1, 25, 252, 141));
  Resource::resources[Resource::cloud] = new PlantableResource("cloud", 2 + 4 * 32, Color::get(-1, 222, 555, 444), Tile::cloud, { Tile::infiniteFall });
  Resource::resources[Resource::gem] = new Resource("gem", 13 + 4 * 32, Color::get(-1, 101, 404, 545));
}