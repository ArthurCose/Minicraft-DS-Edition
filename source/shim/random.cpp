#include "random.h"

#include <time.h>
#include <array>

Random Random::globalRandom = Random();

Random::Random()
{
  generator = std::default_random_engine(time(NULL));
}

Random::Random(long long int seed)
{
  generator = std::default_random_engine(seed);
}

void Random::setSeed(long long int seed)
{
  generator = std::default_random_engine(seed);
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

bool Random::nextBoolean()
{
  return nextInt(2);
}

int Random::nextInt()
{
  auto distribution = std::uniform_int_distribution<int>();
  return distribution(generator);
}

int Random::nextInt(int max)
{
  auto distribution = std::uniform_int_distribution<int>(0, max - 1);
  return distribution(generator);
}

float Random::nextFloat()
{
  auto distribution = std::uniform_real_distribution<float>();
  return distribution(generator);
}

float Random::nextGaussian()
{
  auto distribution = std::normal_distribution<float>(0, 1);
  return distribution(generator);
}
