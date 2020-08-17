#pragma once

class Sprite
{
public:
  int x, y;
  int img;
  int col;
  int bits;

  Sprite(int x, int y, int img, int col, int bits)
  {
    this->x = x;
    this->y = y;
    this->img = img;
    this->col = col;
    this->bits = bits;
  }
};
