#pragma once

#include <vector>
#include <string>
#include "screen.h"
#include "icons.h"

class SoftwareScreen : public Screen
{
private:
  void normalizeBox(int& x, int& y, int& w, int& h);
  void renderSpriteSheetTile(SpriteSheet& spritesheet, int xp, int yp, int tile, uint8_t* colors, int bits);

public:
  uint8_t* pixels;

  SoftwareScreen();
  ~SoftwareScreen();

  void clear(int color) override;

  void renderIcon(int xp, int yp, int tile, int compressedColors, int bits) override;
  void renderTile(int xp, int yp, int tile, std::array<uint8_t, 8> colors, int bits) override;
  void renderPixel(int x, int y, int col) override;
  void renderBox(int x, int y, int w, int h, int col) override;
  void renderBoxFilled(int x, int y, int w, int h, int col) override;
  void flush(void* destination);
};
