#include "lantern.h"

Lantern::Lantern()
  : Furniture("Lantern")
{
  col = Color::get(-1, 000, 111, 555);
  sprite = 5;
  xr = 3;
  yr = 2;
}

int Lantern::getLightRadius()
{
  return 8;
}

std::shared_ptr<Furniture> Lantern::clone()
{
  return std::make_shared<Lantern>();
}
