#pragma once

#include <vector>

class Screen;

struct DarknessBox
{
  int x;
  int y;
  int w;
  int h;
};

class LightMask
{
public:
  const int w, h;
  const int scale;

  int xOffset = 0;
  int yOffset = 0;

  LightMask();

  void reset();
  void setOffset(int xOffset, int yOffset);
  void renderLight(int x, int y, int r);
  void render(Screen &render);

private:
  std::vector<unsigned char> brightness;
  const int brightnessW, brightnessH;

  bool shouldBlock(int scaledX, int scaledY);
  void compressDarkness(
      std::vector<DarknessBox> &boxes,
      int searchX, int searchY, int searchW, int searchH);
  DarknessBox expandDarknessBox(int startX, int startY, int searchW, int searchH);
};
