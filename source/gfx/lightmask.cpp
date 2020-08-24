#include "lightmask.h"

#include <cmath>
#include <nds.h>
#include "screen.h"

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

static inline int nearestPow2(int n)
{
  // not sure why the + 1 is needed
  // in tests on desktop it works without, and would be off with it
  return std::pow(2, std::ceil(std::log2(n))) + 1;
}

LightMask::LightMask(Screen &screen)
    : w(screen.w), h(screen.h), scale(2),
      brightnessW(w / scale + w % scale), brightnessH(h / scale + h % scale),
      texture(screen.genTexture(NULL, nearestPow2(brightnessW), nearestPow2(brightnessH)))
{
  brightness.resize(brightnessW * brightnessH, 0);
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
  x = (x - xOffset) / scale;
  y = (y - yOffset) / scale;
  r /= scale;

  int x0 = x - r;
  int x1 = x + r;
  int y0 = y - r;
  int y1 = y + r;

  if (x0 < 0)
    x0 = 0;
  if (y0 < 0)
    y0 = 0;
  if (x1 > brightnessW)
    x1 = brightnessW;
  if (y1 > brightnessH)
    y1 = brightnessH;

  int rSquared = r * r;

  for (int yy = y0; yy < y1; yy++)
  {
    int yd = yy - y;
    yd = yd * yd;
    for (int xx = x0; xx < x1; xx++)
    {
      int xd = xx - x;
      int dist = xd * xd + yd;

      if (dist <= rSquared)
      {
        int br = 255 - dist * 255 / rSquared;

        size_t index = xx + yy * brightnessW;

        if (brightness[index] < br)
          brightness[index] = br;
      }
    }
  }
}

void LightMask::render(Screen &screen)
{
  // bank swapping based on vramGetBank and glTexImage2D
  // https://github.com/devkitPro/libnds/blob/master/source/arm9/videoGL.c
  auto previousBankModes = VRAM_CR;

  unsigned short *textureData = (unsigned short *)texture->data;
  unsigned short *dataEnd = textureData + texture->width * texture->height;

  if (dataEnd >= VRAM_A && textureData < VRAM_B)
    vramSetBankA(VRAM_A_LCD);
  if (dataEnd >= VRAM_B && textureData < VRAM_C)
    vramSetBankB(VRAM_B_LCD);
  if (dataEnd >= VRAM_C && textureData < VRAM_D)
    vramSetBankC(VRAM_C_LCD);
  if (dataEnd >= VRAM_D && textureData < VRAM_E)
    vramSetBankD(VRAM_D_LCD);

  for (int y = 0; y < brightnessH - 1; y++)
    for (int x = 0; x < brightnessW - 1; x++)
      textureData[y * texture->width + x] = shouldBlock(x, y) ? ARGB16(1, 0, 0, 0) : 0;

  vramRestorePrimaryBanks(previousBankModes);

  screen.renderTexture(*texture, 0, 0, scale, scale);
}

bool LightMask::shouldBlock(int scaledX, int scaledY)
{
  int brightnessIndex = scaledY * brightnessW + scaledX;
  int ditherIndex = ((scaledX + xOffset) & 3) + ((scaledY + yOffset) & 3) * 4;

  return brightness[brightnessIndex] / 10 <= dither[ditherIndex];
}
