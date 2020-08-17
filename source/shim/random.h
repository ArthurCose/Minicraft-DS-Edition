#pragma once

#include <random>

class Random
{
public:
  Random();
  Random(long long int seed);
  void setSeed(long long int seed);
  bool nextBoolean();
  int nextInt();
  int nextInt(int max);
  float nextFloat();
  float nextGaussian();

private:
  std::default_random_engine generator;
  std::uniform_int_distribution<int> uniform_int_distribution;
  std::uniform_real_distribution<float> uniform_float_distribution;
  std::normal_distribution<float> gaussian_distribution;
};
