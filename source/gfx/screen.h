#pragma once

#include <vector>
#include <memory>
#include <string>
#include "spritesheet.h"

class Screen
{
protected:
  Screen(int w, int h);

public:
  static unsigned short palette[256];
  static std::unique_ptr<SpriteSheet> spriteSheet;

  int xOffset = 0;
  int yOffset = 0;

  const int w, h;

  virtual void clear(int color){};
  void setOffset(int xOffset, int yOffset);
  virtual void renderTile(int xp, int yp, int tile, int colors, int bits){};
  void renderText(std::string msg, int x, int y, int col);
  void renderTextCentered(std::string msg, int x, int y, int col);
  void renderFrame(std::string title, int x0, int y0, int x1, int y1);
  virtual void renderPixel(int x, int y, int col){};
  virtual void renderBox(int x, int y, int w, int h, int col){};
  virtual void renderBoxFilled(int x, int y, int w, int h, int col){};
};
