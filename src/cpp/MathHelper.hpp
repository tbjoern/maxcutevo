#pragma once
#include <cassert>
#include <cmath>
#include <random>
#include <vector>

namespace maxcut {

static unsigned int RANDOM_SEED;

struct BernoulliGenerator {
  BernoulliGenerator(double probability, std::mt19937& engine) : b(probability), _engine(engine) {}

  BernoulliGenerator& operator=(BernoulliGenerator&&) = default;

  std::bernoulli_distribution b;
  std::mt19937& _engine;

  bool get() {
    return b(_engine);
  }
};

class MathHelper {
public:
  // static MathHelper& getInstance()
  // {
  //     static MathHelper instance;
  //     return instance;
  // }

  MathHelper() : _engine(RANDOM_SEED), _exp(1), _unif() {}

  MathHelper(MathHelper const &) = delete;
  void operator=(MathHelper const &) = delete;

  void setPowerLawParam(double lambda) {
    _exp.param(std::exponential_distribution<>::param_type(lambda));
  }

  void setUniformRange(int a, int b) {
    _unif.param(std::uniform_int_distribution<>::param_type(a, b));
  }

  void setRealRange(double a, double b) {
    _real.param(std::uniform_real_distribution<>::param_type(a, b));
  }

  int getInt() { return _unif(_engine); }

  int getIntFromPowerLawDistribution(int max) {
    double rand = _exp(_engine);
    rand = rand > max ? max : rand;
    return ceil(rand);
  }

  inline double getReal() { return _real(_engine); }

  template<typename T>
  std::vector<int> chooseKUnique(std::vector<int> &population,
                                 std::vector<T> &weights, int k) {
    std::discrete_distribution d(weights.start(), weights.end());
    std::vector<int> result;
    result.reserve(k);
    for(int i = 0; i < k; ++i) {
      result.push_back(population[d(_engine)]);
    }
    return result;
  }

  double sigmoid(double x) { return (x / 8) / ((abs(x / 2) + 1)) + 1 / 4; }

  inline bool sampleProbability(double probabilty) {
    std::bernoulli_distribution b(probabilty);
    return b(_engine);
  }

  BernoulliGenerator probabilitySampler(double probability) {
    return BernoulliGenerator(probability, _engine);
  }

private:
  std::mt19937 _engine;
  std::exponential_distribution<> _exp;
  std::uniform_int_distribution<> _unif;
  std::uniform_real_distribution<> _real;
};

} // namespace maxcut
