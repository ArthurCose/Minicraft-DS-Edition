#pragma once

#include <vector>
#include "../../fixedpoint.h"

struct GeneratedLevel
{
  static constexpr fixed32 DEFAULT_TICK_RATE = 1.0 / 50.0;
  static constexpr int DEFAULT_MONSTER_DENSITY = 8;

  std::vector<unsigned char> map;
  std::vector<unsigned char> data;
  int monsterDensity = 8;
  fixed32 tickRate = DEFAULT_TICK_RATE;
};
