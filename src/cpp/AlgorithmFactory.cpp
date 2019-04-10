#include "AlgorithmFactory.hpp"
#include "algorithm/Greedy.hpp"
#include "algorithm/PMUT.hpp"
#include "algorithm/PMUTActivity.hpp"
#include "algorithm/Unif.hpp"

using namespace std;

namespace maxcut {
std::shared_ptr<Algorithm>
AlgorithmFactory::make(std::string algorithm_name, const AdjList &adj_list,
                       const nlohmann::json &params) {
  auto type = str_to_algorithm_type.at(algorithm_name);

  double power_law_beta;
  switch (type) {
  case Algorithm_Type::GREEDY:
    return make_shared<Greedy>(adj_list);
  case Algorithm_Type::PMUT:
    power_law_beta = params["power_law_beta"];
    return make_shared<PMUT>(adj_list, power_law_beta);
  case Algorithm_Type::PMUTACTIVITY:
    power_law_beta = params["power_law_beta"];
    return make_shared<PMUTActivity>(adj_list, power_law_beta);
  case Algorithm_Type::UNIF:
    return make_shared<Unif>(adj_list);
  }

  throw runtime_error("Unknown algorithm name");
}
} // namespace maxcut
