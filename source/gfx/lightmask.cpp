#include "lightmask.h"

#include <cmath>
#include <algorithm>
#include <nds.h>
#include "screen.h"
#include "../game.h"

// positive numbers only
static inline int divCeil(int a, int b)
{
  return 1 + (a - 1) / b;
}

static inline int nearestPow2(int n)
{
  // not sure why the + 1 is needed
  // in tests on desktop it works without, and would be off with it
  return std::pow(2, std::ceil(std::log2(n))) + 1;
}

static const int SCALE = 3;
static const int MAX_LIGHT_RADIUS = 8 * 8 / SCALE;
static const int TILE_LEN = divCeil(16, SCALE);

static const int dither[16] = {
    0,
    8,
    2,
    10,
    12,
    4,
    14,
    6,
    3,
    11,
    1,
    9,
    15,
    7,
    13,
    5,
};

static constexpr void precalculateLight(int r, uint16_t* precalculated) {
  // expects r to be divided by SCALE before being called

  int sideLen = r * 2;
  int rSquared = r * r;

  for (int y = 0; y < sideLen; y++) {
    int yd = y - r;
    yd = yd * yd;

    for (int x = 0; x < sideLen; x++) {
      int xd = x - r;
      int dist = xd * xd + yd;

      size_t index = x + y * sideLen;
      int brightness = 255 - dist * 255 / rSquared;

      if (brightness > 0) {
        precalculated[index] = brightness;
      }
    }
  }
}

static constexpr size_t precalculatedLightOffset(int r) {
  size_t size = 0;

  for (int i = 1; i < r; i++) {
    size_t sideLen = i + i;
    size += sideLen * sideLen;
  }

  return size;
}

static const int PRECALCULATED_LIGHTS_SIZE = precalculatedLightOffset(MAX_LIGHT_RADIUS + 1);

static constexpr std::array<uint16_t, PRECALCULATED_LIGHTS_SIZE> precalculateLights() {
  std::array<uint16_t, PRECALCULATED_LIGHTS_SIZE> lights = {};

  for (int i = 0; i <= MAX_LIGHT_RADIUS; i++) {
    precalculateLight(i, &lights[precalculatedLightOffset(i)]);
  }

  return lights;
}

static constexpr std::array<size_t, MAX_LIGHT_RADIUS> precalculateLightOffsets() {
  std::array<size_t, MAX_LIGHT_RADIUS> offsets = {};

  for (int i = 0; i < MAX_LIGHT_RADIUS; i++) {
    offsets[i] = precalculatedLightOffset(i + 1);
  }

  return offsets;
}

static const std::array<uint16_t, PRECALCULATED_LIGHTS_SIZE> PRECALCULATED_LIGHTS = precalculateLights();
static const std::array<size_t, MAX_LIGHT_RADIUS> PRECALCULATED_LIGHT_OFFSETS = precalculateLightOffsets();

LightMask::LightMask(Screen& screen)
  : w(screen.w), h(Game::WORLD_SCREEN_HEIGHT),
  brightnessW(w / SCALE),
  brightnessH(h / SCALE)
{
  brightness.resize(brightnessW * brightnessH, 0);

  int textureW = nearestPow2(brightnessW);
  int textureH = nearestPow2(brightnessH);

  // force textureB into bank B
  glLockVRAMBank(VRAM_A);
  textureB = screen.genTexture(NULL, textureW, textureH);

  // force textureD into bank D
  glLockVRAMBank(VRAM_B);
  glLockVRAMBank(VRAM_C);
  textureD = screen.genTexture(NULL, textureW, textureH);
  glLockVRAMBank(VRAM_D);

  // keep bank B and D locked, but unlock the rest
  glUnlockVRAMBank(VRAM_A);
  glUnlockVRAMBank(VRAM_C);
}

void LightMask::reset()
{
  brightness.assign(brightness.size(), 0);
}

void LightMask::setOffset(int xOffset, int yOffset)
{
  this->xOffset = xOffset;
  this->yOffset = yOffset;
}

void LightMask::renderLight(int x, int y, int r)
{
  x = (x - xOffset) / SCALE;
  y = (y - yOffset) / SCALE;
  r /= SCALE;

  r = std::clamp(r, 1, MAX_LIGHT_RADIUS);
  auto lightOffset = PRECALCULATED_LIGHT_OFFSETS[r - 1];

  int x0 = x - r;
  int x1 = x + r;
  int y0 = y - r;
  int y1 = y + r;

  x0 = std::max(x0, 0);
  y0 = std::max(y0, 0);
  x1 = std::min(x1, brightnessW);
  y1 = std::min(y1, brightnessH);

  int pxOffset = (x0 - (x - r)) - x0;
  int pyOffset = (y0 - (y - r)) - y0;
  int sideLen = r * 2;

  for (int yy = y0; yy < y1; yy++) {
    for (int xx = x0; xx < x1; xx++) {
      size_t pIndex = (xx + pxOffset) + (yy + pyOffset) * sideLen;
      int br = PRECALCULATED_LIGHTS[pIndex + lightOffset];

      size_t index = xx + yy * brightnessW;

      if (brightness[index] < br)
        brightness[index] = br;
    }
  }
}

void LightMask::fillTile(int x, int y)
{
  int x0 = (x * 16 - xOffset) / SCALE;
  int y0 = (y * 16 - yOffset) / SCALE;
  int x1 = x0 + TILE_LEN;
  int y1 = y0 + TILE_LEN;

  x0 = std::max(x0, 0);
  y0 = std::max(y0, 0);
  x1 = std::min(x1, brightnessW);
  y1 = std::min(y1, brightnessH);

  for (int yy = y0; yy < y1; yy++)
    for (int xx = x0; xx < x1; xx++)
      brightness[xx + yy * brightnessW] = 255;
}

void LightMask::render(Screen& screen)
{
  Texture& texture = usingTextureB ? *textureB : *textureD;

  unsigned short* textureData = (unsigned short*)texture.data;

  auto previousBankModes = VRAM_CR;

  if (usingTextureB)
    vramSetBankB(VRAM_B_LCD);
  else
    vramSetBankD(VRAM_D_LCD);

  unsigned short shadowColor = Screen::palette[0] | 1 << 15;

  for (int y = 0; y < brightnessH; y++) {
    for (int x = 0; x < brightnessW; x++) {
      textureData[y * texture.width + x] = shouldBlock(x, y) ? shadowColor : 0;
    }
  }

  DC_FlushRange(texture.data, texture.width * texture.height);
  vramRestorePrimaryBanks(previousBankModes);

  screen.renderTexture(texture, 0, 0, SCALE, SCALE);

  usingTextureB = !usingTextureB;
}

bool LightMask::shouldBlock(int scaledX, int scaledY)
{
  int brightnessIndex = scaledY * brightnessW + scaledX;
  int ditherIndex = ((scaledX + xOffset) & 3) + ((scaledY + yOffset) & 3) * 4;

  return brightness[brightnessIndex] >> 3 <= dither[ditherIndex];
}
