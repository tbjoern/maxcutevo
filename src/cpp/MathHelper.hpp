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

  BernoulliGenerator(std::bernoulli_distribution _b, std::mt19937 &engine)
      : b(_b), _engine(engine) {}

  bool get() { return b(_engine); }
};

struct PowerLawGenerator {
  std::vector<double> VWeights;
  std::map<double, int> TV;
  std::mt19937 &_engine;
  std::uniform_real_distribution<double> dist;

  PowerLawGenerator(int variables, double beta, std::mt19937 &engine)
      : _engine(engine) {
    VWeights.resize(variables);
    double VSum = 0;

    // calculating the exponents for power-law weights
    double betaNorm = 1 / (beta - 1);

    // mapping variables to weights and creating a 'cumulative distribution'
    for (int i = 0; i < variables; ++i) {
      VWeights[i] = pow((variables / (i + 1)), betaNorm);
      VSum += VWeights[i];
      TV[VSum] = i;
    }

    dist = std::uniform_real_distribution<double>(0.0, VSum);
  }

  int get() {
    double v = dist(_engine);
    auto var = TV.lower_bound(v)->second;
    return var;
  }
};

class MathHelper {
public:
  // static MathHelper& getInstance()
  // {
  //     static MathHelper instance;
  //     return instance;
  // }

  MathHelper() : _engine(std::random_device{}()), _exp(1), _unif() {}

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

  double sigmoid(double x, double smoothness) {
    return 2 / (1 + exp(-1 * smoothness * x));
  }

  inline bool sampleProbability(double probabilty) {
    std::bernoulli_distribution b(probabilty);
    return b(_engine);
  }

  std::shared_ptr<BernoulliGenerator> probabilitySampler(double probability) {
    return std::make_unique<BernoulliGenerator>(
        std::bernoulli_distribution(probability), _engine);
  }

  std::mt19937 &getEngine() { return _engine; }

private:
  std::mt19937 _engine;
  std::exponential_distribution<> _exp;
  std::uniform_int_distribution<> _unif;
  std::uniform_real_distribution<> _real;
};

} // namespace maxcut
