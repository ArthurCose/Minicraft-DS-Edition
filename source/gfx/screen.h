#pragma once

#include <vector>
#include <memory>
#include <string>
#include "spritesheet.h"

class Screen
{
  /*
	 * public static final int MAP_WIDTH = 64; // Must be 2^x public static final int MAP_WIDTH_MASK = MAP_WIDTH - 1;
	 * 
	 * public int[] tiles = new int[MAP_WIDTH * MAP_WIDTH]; public int[] colors = new int[MAP_WIDTH * MAP_WIDTH]; public int[] databits = new int[MAP_WIDTH * MAP_WIDTH];
	 */
public:
  static unsigned short palette[256];
  static std::unique_ptr<SpriteSheet> spriteSheet;

  int xOffset = 0;
  int yOffset = 0;

  const int w, h;

  Screen();

  void clear(int color);
  void setOffset(int xOffset, int yOffset);
  void renderTile(int xp, int yp, int tile, int colors, int bits);
  void renderText(std::string msg, int x, int y, int col);
  void renderTextCentered(std::string msg, int x, int y, int col);
  void renderFrame(std::string title, int x0, int y0, int x1, int y1);
  void renderPixel(int x, int y, int col);
  void renderBox(int x, int y, int w, int h, int col);
  void renderBoxFilled(int x, int y, int w, int h, int col);
};
