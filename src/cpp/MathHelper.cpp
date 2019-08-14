#include "MathHelper.hpp"

namespace maxcut {

BernoulliGenerator::BernoulliGenerator(std::bernoulli_distribution _b,
                                       std::mt19937 &engine)
    : b(_b), _engine(engine) {}

bool BernoulliGenerator::get() { return b(_engine); }

PowerLawGenerator::PowerLawGenerator(int variables, double beta,
                                     std::mt19937 &engine)
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

int PowerLawGenerator::get() {
  double v = dist(_engine);
  auto var = TV.lower_bound(v)->second;
  return var;
}

int maxcut::MathHelper::SEED = 0;

int MathHelper::get_seed() {
  if (SEED == 0) {
    return std::random_device{}();
  } else {
    return SEED;
  }
}

MathHelper::MathHelper() : _engine(get_seed()), _exp(1), _unif() {}

void MathHelper::setPowerLawParam(double lambda) {
  _exp.param(std::exponential_distribution<>::param_type(lambda));
}

void MathHelper::setUniformRange(int a, int b) {
  _unif.param(std::uniform_int_distribution<>::param_type(a, b));
}

void MathHelper::setRealRange(double a, double b) {
  _real.param(std::uniform_real_distribution<>::param_type(a, b));
}

int MathHelper::getInt() { return _unif(_engine); }

PowerLawGenerator MathHelper::makePowerLawGenerator(int max_value,
                                                    double beta) {
  return PowerLawGenerator(max_value, beta, _engine);
}

inline double MathHelper::getReal() { return _real(_engine); }

double MathHelper::sigmoid(double x, double smoothness) {
  return 2 / (1 + exp(-1 * smoothness * x));
}

std::shared_ptr<BernoulliGenerator>
MathHelper::probabilitySampler(double probability) {
  return std::make_unique<BernoulliGenerator>(
      std::bernoulli_distribution(probability), _engine);
}

std::mt19937 &MathHelper::getEngine() { return _engine; }

} // namespace maxcut
