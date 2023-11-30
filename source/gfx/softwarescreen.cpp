#include "softwarescreen.h"

#include <nds.h>
#include <algorithm>
#include <cctype>
#include "color.h"
#include "iconsheet.h"

SoftwareScreen::SoftwareScreen()
  : Screen(SCREEN_WIDTH, SCREEN_HEIGHT)
{
  pixels = (uint8_t*)aligned_alloc(4, SCREEN_WIDTH * SCREEN_HEIGHT);
}

SoftwareScreen::~SoftwareScreen()
{
  free(pixels);
}

void SoftwareScreen::clear(int color)
{
  size_t size = w * h;

  // unsigned int unsignedCol = color;
  unsigned int word = color |
    (color << 8) |
    (color << 16) |
    (color << 24);

  // fill in the last 4 bytes to avoid alignment issues
  for (size_t i = 0; i < 4; i++) {
    pixels[size - i - 1] = color;
  }

  while (dmaBusy(3)) {}

  dmaFillWords(word, pixels, size - 3);
}

void SoftwareScreen::renderSpriteSheetTile(SpriteSheet& spritesheet, int xp, int yp, int tile, uint8_t* colors, int bits)
{
  const int TILE_W = spritesheet.tileW;
  const int BITMAP_W = spritesheet.w;
  const int ROW_TILE_STRIDE = BITMAP_W / TILE_W;
  const int TILE_X = tile % ROW_TILE_STRIDE;
  const int TILE_Y = tile / ROW_TILE_STRIDE;
  const int TILE_OFFSET = TILE_X * TILE_W + TILE_Y * TILE_W * BITMAP_W;

  bool mirrorX = (bits & BIT_MIRROR_X) > 0;
  bool mirrorY = (bits & BIT_MIRROR_Y) > 0;

  xp -= xOffset;
  yp -= yOffset;

  int x0 = std::max(std::max(xp, 0) - xp, 0);
  int y0 = std::max(std::max(yp, 0) - yp, 0);
  int x1 = std::min(std::min(xp + TILE_W, w) - xp, TILE_W);
  int y1 = std::min(std::min(yp + TILE_W, h) - yp, TILE_W);

  for (int y = y0; y < y1; y++) {
    int ys = y;

    if (mirrorY) {
      ys = TILE_W - y - 1;
    }

    for (int x = x0; x < x1; x++) {
      int xs = x;

      if (mirrorX) {
        xs = TILE_W - x - 1;
      }

      int bitmapIndex = ys * BITMAP_W + xs + TILE_OFFSET;
      int colorIndex = spritesheet.bitmap[bitmapIndex / 2] >> (bitmapIndex % 2 * 4) & 15;

      if (colorIndex == 0) {
        continue;
      }

      int col = colors[colorIndex - 1];

      if (col < 255) {
        pixels[(x + xp) + (y + yp) * w] = col;
      }
    }
  }
}

void SoftwareScreen::renderIcon(int x, int y, int tile, int compressedColors, int bits)
{
  renderSpriteSheetTile(*icons, x, y, tile, (uint8_t*)&compressedColors, bits);
}

void SoftwareScreen::renderTile(int x, int y, int tile, std::array<uint8_t, 8> colors, int bits)
{
  renderSpriteSheetTile(*tiles, x, y, tile, (uint8_t*)&colors, bits);
}

void SoftwareScreen::renderPixel(int x, int y, int col)
{
  x -= xOffset;
  y -= yOffset;

  if (x >= 0 && x < w && y >= 0 && y < h) {
    pixels[y * w + x] = col;
  }
}

void SoftwareScreen::normalizeBox(int& x, int& y, int& w, int& h)
{
  // convert negative sizes into positive sizes
  if (w < 0) {
    x += w;
    w = -w;
  }
  if (h < 0) {
    y += h;
    h = -h;
  }

  // make sure box is within screen
  if (x < 0) {
    w += x;
    x = 0;
  }
  if (y < 0) {
    h += y;
    y = 0;
  }
  if (x + w > this->w) {
    w = this->w - x;
  }
  if (y + h > this->h) {
    h = this->h - y;
  }
}

void SoftwareScreen::renderBox(int x, int y, int w, int h, int col)
{
  x -= xOffset;
  y -= yOffset;

  normalizeBox(x, y, w, h);

  for (int i = 0; i < w; i++) {
    pixels[y * this->w + x + i] = col;
    pixels[(y + h - 1) * this->w + x + i] = col;
  }

  for (int i = 0; i < h; i++) {
    pixels[(y + i) * this->w + x] = col;
    pixels[(y + i) * this->w + x + w - 1] = col;
  }
}

void SoftwareScreen::renderBoxFilled(int x, int y, int w, int h, int col)
{
  x -= xOffset;
  y -= yOffset;

  normalizeBox(x, y, w, h);

  int right = x + w;
  int bottom = y + h;

  for (int i = y; i < bottom; i++) {
    for (int j = x; j < right; j++) {
      pixels[i * this->w + j] = col;
    }
  }
}

void SoftwareScreen::flush(void* destination)
{
  // wait until work is done
  while (dmaBusy(0) || dmaBusy(1) || dmaBusy(2) || dmaBusy(3)) {
  }

  DC_FlushRange(pixels, w * h);
  dmaCopy(pixels, destination, w * h);
}
