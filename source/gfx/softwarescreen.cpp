#include "softwarescreen.h"

#include <nds.h>
#include <cctype>
#include "color.h"

SoftwareScreen::SoftwareScreen()
  : Screen(SCREEN_WIDTH, SCREEN_HEIGHT)
{
  pixels = (unsigned char*)aligned_alloc(4, SCREEN_WIDTH * SCREEN_HEIGHT);
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

void SoftwareScreen::renderTile(int xp, int yp, int tile, int compressedColors, int bits)
{
  xp -= xOffset;
  yp -= yOffset;

  bool mirrorX = (bits & BIT_MIRROR_X) > 0;
  bool mirrorY = (bits & BIT_MIRROR_Y) > 0;

  int colors[5] = {
      255,
      (compressedColors >> (0 * 8)) & 255,
      (compressedColors >> (1 * 8)) & 255,
      (compressedColors >> (2 * 8)) & 255,
      (compressedColors >> (3 * 8)) & 255
  };

  for (int y = 0; y < 8; y++) {
    int ys = y;
    if (mirrorY)
      ys = 7 - y;
    if (y + yp < 0 || y + yp >= h)
      continue;
    for (int x = 0; x < 8; x++) {
      if (x + xp < 0 || x + xp >= w)
        continue;

      int xs = x;
      if (mirrorX)
        xs = 7 - x;

      int colorIndex = spriteSheet->resolvePixelColorIndex(xs, ys, tile);

      int col = colors[colorIndex];

      if (col < 255)
        pixels[(x + xp) + (y + yp) * w] = col;
    }
  }
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
