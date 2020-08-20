#pragma once

#include <vector>
#include <string>
#include "screen.h"

class SoftwareScreen : public Screen
{
private:
  void normalizeBox(int &x, int &y, int &w, int &h);

public:
  std::vector<unsigned char> pixels;

  SoftwareScreen();

  void clear(int color) override;
  void renderTile(int xp, int yp, int tile, int colors, int bits) override;
  void renderPixel(int x, int y, int col) override;
  void renderBox(int x, int y, int w, int h, int col) override;
  void renderBoxFilled(int x, int y, int w, int h, int col) override;
};
