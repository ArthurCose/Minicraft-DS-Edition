#pragma once

#include <random>

class Random
{
public:
  static Random globalRandom;

  Random();
  Random(long long int seed);
  void setSeed(long long int seed);
  int nextBits(int bits);
  bool nextBoolean();
  int nextInt();
  int nextInt(int max);
  float nextFloat();
  float nextGaussian();

private:
  std::default_random_engine generator;
  std::normal_distribution<float> gaussian_distribution = std::normal_distribution<float>(-1, 1);
};
