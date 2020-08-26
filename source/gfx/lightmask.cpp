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
    : w(screen.w), h(screen.h), scale(3),
      brightnessW(std::ceil(w / scale)), brightnessH(std::ceil(h / scale))
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

void LightMask::fillTile(int x, int y)
{
  int x0 = (x * 16 - xOffset) / scale;
  int y0 = (y * 16 - yOffset) / scale;
  int x1 = x0 + std::ceil(16 / scale);
  int y1 = y0 + std::ceil(16 / scale);

  if (x0 < 0)
    x0 = 0;
  if (y0 < 0)
    y0 = 0;
  if (x1 > brightnessW)
    x1 = brightnessW;
  if (y1 > brightnessH)
    y1 = brightnessH;

  for (int yy = y0; yy < y1; yy++)
    for (int xx = x0; xx < x1; xx++)
      brightness[xx + yy * brightnessW] = 255;
}

void LightMask::render(Screen &screen)
{
  std::shared_ptr<Texture> texture = usingTextureB ? textureB : textureD;

  unsigned short *textureData = (unsigned short *)texture->data;

  auto previousBankModes = VRAM_CR;

  if (usingTextureB)
    vramSetBankB(VRAM_B_LCD);
  else
    vramSetBankD(VRAM_D_LCD);

  unsigned short shadowColor = Screen::palette[0] | 1 << 15;

  for (int y = 0; y < brightnessH - 1; y++)
    for (int x = 0; x < brightnessW - 1; x++)
      textureData[y * texture->width + x] = shouldBlock(x, y) ? shadowColor : 0;

  DC_FlushRange(texture->data, texture->width * texture->height);
  vramRestorePrimaryBanks(previousBankModes);

  screen.renderTexture(*texture, 0, 0, scale, scale);

  usingTextureB = !usingTextureB;
}

bool LightMask::shouldBlock(int scaledX, int scaledY)
{
  int brightnessIndex = scaledY * brightnessW + scaledX;
  int ditherIndex = ((scaledX + xOffset) & 3) + ((scaledY + yOffset) & 3) * 4;

  return brightness[brightnessIndex] / 10 <= dither[ditherIndex];
}
