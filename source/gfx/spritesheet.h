#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <gl2d.h>
#include "tilepalette.h"

class Screen;

class SpriteSheet
{
private:
  std::vector<glImage> tiles;
  std::unordered_map<int, std::unique_ptr<TilePalette>> palettes;
  int lastBoundPalette = -1;
  int textureId;

public:
  const int w, h, tileW;

  SpriteSheet(const unsigned char *bitmap, int width, int height, int tileW);
  void renderTile(Screen &screen, int xp, int yp, int tile, int colors, int bits);
};