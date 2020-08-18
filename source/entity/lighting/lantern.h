#pragma once

#include "../furniture.h"

class Lantern : public Furniture
{
public:
  Lantern();

  int getLightRadius() override;
  std::shared_ptr<Furniture> clone() override;
};
