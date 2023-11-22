#include "random.h"

#include <time.h>

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
}

bool Random::nextBoolean()
{
  return uniform_int_distribution(generator) % 2;
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
