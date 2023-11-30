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

public:
  const unsigned char* bitmap;
  const int w, h, tileW, textureId;

  SpriteSheet(const unsigned char* bitmap, int width, int height, int tileW);

  const glImage* tileGlImage(int tile) {
    return &tiles[tile];
  }
};
