#pragma once

#include <vector>
#include <memory>
#include <string>
#include "screen.h"
#include "spritesheet.h"

class GLScreen : public Screen
{
public:
  GLScreen();

  void clear(int color) override;
  void renderTile(int xp, int yp, int tile, int colors, int bits) override;
  void renderPixel(int x, int y, int col) override;
  void renderBox(int x, int y, int w, int h, int col) override;
  void renderBoxFilled(int x, int y, int w, int h, int col) override;
  std::shared_ptr<Texture> genTexture(void* loadData, int w, int h);
  void renderTexture(Texture& texture, int x, int y, int scaleX, int scaleY) override;
};
