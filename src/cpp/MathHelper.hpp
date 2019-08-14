#pragma once
#include <cassert>
#include <cmath>
#include <functional>
#include <map>
#include <memory>
#include <random>
#include <unordered_map>
#include <vector>

namespace maxcut {

struct BernoulliGenerator {
  std::bernoulli_distribution b;
  std::mt19937 &_engine;

  BernoulliGenerator(std::bernoulli_distribution _b, std::mt19937 &engine);

  bool get();
};

struct PowerLawGenerator {
  std::vector<double> VWeights;
  std::map<double, int> TV;
  std::mt19937 &_engine;
  std::uniform_real_distribution<double> dist;

  PowerLawGenerator(int variables, double beta, std::mt19937 &engine);

  int get();
};

class MathHelper {
public:
  static int SEED;

  static int get_seed();

  MathHelper();

  MathHelper(MathHelper const &) = delete;
  void operator=(MathHelper const &) = delete;

  void setPowerLawParam(double lambda);

  void setUniformRange(int a, int b);

  void setRealRange(double a, double b);

  int getInt();

  PowerLawGenerator makePowerLawGenerator(int max_value, double beta);

  inline double getReal();

  template <typename T>
  std::vector<int> chooseKUnique(std::vector<T> &weights, int k) {
    std::discrete_distribution<> d(weights.begin(), weights.end());
    std::vector<int> result;
    result.reserve(k);
    for (int i = 0; i < k; ++i) {
      result.push_back(d(_engine));
    }
    return result;
  }

  double sigmoid(double x, double smoothness);

  inline bool sampleProbability(double probabilty) {
    std::bernoulli_distribution b(probabilty);
    return b(_engine);
  }

  std::shared_ptr<BernoulliGenerator> probabilitySampler(double probability);

  std::mt19937 &getEngine();

private:
  std::mt19937 _engine;
  std::exponential_distribution<> _exp;
  std::uniform_int_distribution<> _unif;
  std::uniform_real_distribution<> _real;
};

} // namespace maxcut
