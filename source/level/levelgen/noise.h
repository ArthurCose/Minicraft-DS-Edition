#include <vector>
#include "../../shim/random.h"

class Noise
{
public:
  const int w, h;
  std::vector<double> values;

  Noise(Random& random, int w, int h, int featureSize);

private:
  double sample(int x, int y);
  void setSample(int x, int y, double value);
};
