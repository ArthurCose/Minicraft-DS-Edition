#pragma once

#include <vector>
#include <memory>
#include <gl2d.h>
#include "tilepalette.h"

class GLScreen;
class SoftwareScreen;

class SpriteSheet
{
private:
  std::vector<glImage> tiles;
  const unsigned char* bitmap;

public:
  const int w, h, tileW, textureId;

  SpriteSheet(const unsigned char* bitmap, int width, int height, int tileW);

  const glImage* tileGlImage(int tile) {
    return &tiles[tile];
  }

  int resolvePixelColorIndex(int x, int y, int tile) {
    int tileX = tile % 32;
    int tileY = tile / 32;
    int tileOffset = tileX * 8 + tileY * 8 * w;

    int bitmapIndex = y * w + x + tileOffset;

    return bitmap[bitmapIndex / 2] >> (bitmapIndex % 2 * 4) & 15;
  }
};
