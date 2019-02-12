#pragma once
#include <cassert>
#include <cmath>
#include <random>
#include <vector>

namespace maxcut {

static unsigned int RANDOM_SEED;

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

  std::vector<int> chooseKUnique(std::vector<int> &population,
                                 std::vector<int> &weights, int k) {
    std::vector<int> result;
    assert(population.size() == weights.size());
    int total_weight = 0;
    for (auto &v : weights) {
      total_weight += v;
    }
    std::vector<unsigned int> node_chosen(population.size(), false);
    for (int i = 0; i < k; ++i) {
      setUniformRange(0, total_weight);
      int r = _unif(_engine);
      int cumulative_weight = 0;
      for (int j = 0; j < population.size(); ++j) {
        if (!node_chosen[j]) {
          cumulative_weight += weights[j];
          if (cumulative_weight >= r) {
            result.push_back(population[j]);
            node_chosen[j] = true;
            total_weight -= weights[j];
            break;
          }
        }
      }
    }
    return result;
  }

  std::vector<int> chooseKUnique(std::vector<int> &population,
                                 std::vector<double> &weights, int k) {
    std::vector<int> result;
    assert(population.size() == weights.size());
    double total_weight = 0;
    for (auto &v : weights) {
      total_weight += v;
    }
    std::vector<unsigned int> node_chosen(population.size(), false);
    for (int i = 0; i < k; ++i) {
      setRealRange(0, total_weight);
      auto r = getReal();
      double cumulative_weight = 0;
      for (int j = 0; j < population.size(); ++j) {
        if (!node_chosen[j]) {
          cumulative_weight += weights[j];
          if (cumulative_weight >= r) {
            result.push_back(population[j]);
            node_chosen[j] = true;
            total_weight -= weights[j];
            break;
          }
        }
      }
    }
    return result;
  }

  double sigmoid(double x) { return (x / 8) / ((abs(x / 2) + 1)) + 1 / 4; }

  inline bool sampleProbability(double probabilty) {
    return getReal() <= probabilty;
  }

private:
  std::mt19937 _engine;
  std::exponential_distribution<> _exp;
  std::uniform_int_distribution<> _unif;
  std::uniform_real_distribution<> _real;
};

} // namespace maxcut
