#include "AlgorithmFactory.hpp"
#include "algorithm/Greedy.hpp"
#include "algorithm/PMUT.hpp"
#include "algorithm/PMUTActivity.hpp"
#include "algorithm/Unif.hpp"
#include "algorithm/UnifSigmoid.hpp"

using namespace std;

namespace {
maxcut::ActivityAlgorithm::Parameters
make_parameters(const nlohmann::json &config) {
  maxcut::ActivityAlgorithm::Parameters params;
  if (config.find("start_activity") != config.end()) {
    params.START_ACTIVITY = config["start_activity"];
  }
  if (config.find("min") != config.end()) {
    params.ACT_MIN = config["min"];
  }
  if (config.find("max") != config.end()) {
    params.ACT_MAX = config["max"];
  }
  if (config.find("inc") != config.end()) {
    params.ACT_INC = config["inc"];
  }
  if (config.find("dec") != config.end()) {
    params.ACT_DEC = config["dec"];
  }
  if (config.find("decay_rate") != config.end()) {
    params.DECAY_RATE = config["decay_rate"];
  }
  return params;
}
} // namespace

namespace maxcut {
std::shared_ptr<Algorithm>
AlgorithmFactory::make(std::string algorithm_name, const AdjList &adj_list,
                       const nlohmann::json &params) {
  auto type = str_to_algorithm_type.at(algorithm_name);

  double power_law_beta;
  double sigmoid_smoothness;
  switch (type) {
  case Algorithm_Type::GREEDY:
    return make_shared<Greedy>(adj_list);
  case Algorithm_Type::PMUT:
    power_law_beta = params["power_law_beta"];
    return make_shared<PMUT>(adj_list, power_law_beta);
  case Algorithm_Type::PMUTACTIVITY:
    power_law_beta = params["power_law_beta"];
    return make_shared<PMUTActivity>(adj_list, make_parameters(params),
                                     power_law_beta);
  case Algorithm_Type::UNIF:
    return make_shared<Unif>(adj_list);
  case Algorithm_Type::UNIFSIGMOID:
    sigmoid_smoothness = params["sigmoid_smoothness"];
    return make_shared<UnifSigmoid>(adj_list, make_parameters(params),
                                    sigmoid_smoothness);
  }

  throw runtime_error("Unknown algorithm name");
}
} // namespace maxcut
