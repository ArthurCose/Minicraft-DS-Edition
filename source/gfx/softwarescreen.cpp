#include "softwarescreen.h"

#include <nds.h>
#include <cctype>
#include "color.h"

SoftwareScreen::SoftwareScreen()
  : Screen(SCREEN_WIDTH, SCREEN_HEIGHT)
{
  pixels.resize(SCREEN_WIDTH * SCREEN_HEIGHT, 0);
}

void SoftwareScreen::clear(int color)
{
  size_t size = w * h;
  size_t leftoverBytes = size % 4;

  // unsigned int unsignedCol = color;
  unsigned int word = color |
    (color << 8) |
    (color << 16) |
    (color << 24);

  for (size_t i = size - leftoverBytes; i < size; i++) {
    pixels[i] = color;
  }

  while (dmaBusy(3)) {}

  dmaFillWords(word, &pixels[0], w * h);
}

void SoftwareScreen::renderTile(int xp, int yp, int tile, int compressedColors, int bits)
{
  spriteSheet->renderTile(*this, xp, yp, tile, compressedColors, bits);
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

  if (w * h < 256) {
    for (int i = y; i < bottom; i++) {
      for (int j = x; j < right; j++) {
        pixels[i * this->w + j] = col;
      }
    }
  } else {
    int leftoverBytes = w % 4;
    unsigned int word = col |
      (col << 8) |
      (col << 16) |
      (col << 24);

    while (dmaBusy(3)) {}

    for (int i = y; i < y + h; i++) {
      for (int j = x + w - leftoverBytes; j < right; j++) {
        pixels[i * this->w + j] = col;
      }

      dmaFillWords(word, &pixels[i * this->w + x], w);
    }
  }
}
