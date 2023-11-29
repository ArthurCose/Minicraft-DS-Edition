#pragma once

#include <vector>
#include <string>
#include "screen.h"
#include "icons.h"

class SoftwareScreen : public Screen
{
private:
  void normalizeBox(int& x, int& y, int& w, int& h);

public:
  unsigned char* pixels;

  SoftwareScreen();
  ~SoftwareScreen();

  void clear(int color) override;

  void renderIcon(int xp, int yp, int tile, int compressedColors, int bits) override;
  void renderPixel(int x, int y, int col) override;
  void renderBox(int x, int y, int w, int h, int col) override;
  void renderBoxFilled(int x, int y, int w, int h, int col) override;
  void flush(void* destination);
};
