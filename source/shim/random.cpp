#include "random.h"

#include <time.h>
#include <array>

static std::uniform_real_distribution<> dis(1.0, 2.0);

Random Random::globalRandom = Random();

Random::Random()
  : gaussian_distribution(-1, 1)
{
  generator = std::default_random_engine(time(NULL));
}

Random::Random(long long int seed)
  : gaussian_distribution(-1, 1)
{
  generator = std::default_random_engine(seed);
}

void Random::setSeed(long long int seed)
{
  generator = std::default_random_engine(seed);
  remainingBits = 0;
}

inline constexpr auto genMasks() {
  std::array<int, 32> masks{};

  int mask = 0;

  for (size_t i = 0; i < masks.size(); i++) {
    masks[i] = mask;
    mask <<= 1;
    mask |= 1;
  }

  return masks;
}

int Random::nextBits(int bits)
{
  const auto MASKS = genMasks();
  int result = 0;

  while (bits >= remainingBits) {
    result <<= remainingBits;
    result = bitStorage & MASKS[remainingBits];
    bitStorage = nextInt();
    remainingBits = 32;
    bits -= remainingBits;
  }

  if (bits > 0) {
    result <<= bits;
    result = bitStorage & MASKS[bits];
    bitStorage >>= bits;
    remainingBits -= bits;
  }

  return result;
}

bool Random::nextBoolean()
{
  return nextBits(1);
}

int Random::nextInt()
{
  return uniform_int_distribution(generator);
}

int Random::nextInt(int max)
{
  return uniform_int_distribution(generator) % max;
}

float Random::nextFloat()
{
  return uniform_float_distribution(generator);
}

float Random::nextGaussian()
{
  return gaussian_distribution(generator);
}
