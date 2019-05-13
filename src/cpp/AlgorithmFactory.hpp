#pragma once
#include "algorithm/Algorithm.hpp"
#include "types.hpp"
#include <memory>
#include <unordered_map>

namespace maxcut {

enum class Algorithm_Type {
  UNIF,
  PMUT,
  FMUT,
  PMUTACTIVITY,
  GREEDY,
  RANDOM,
  UNIFSIGMOID
};

const std::unordered_map<std::string, Algorithm_Type> str_to_algorithm_type = {
    {"unif", Algorithm_Type::UNIF},
    {"pmut", Algorithm_Type::PMUT},
    {"fmut", Algorithm_Type::FMUT},
    {"pmutActivity", Algorithm_Type::PMUTACTIVITY},
    {"greedy", Algorithm_Type::GREEDY},
    {"random", Algorithm_Type::RANDOM},
    {"unifSigmoid", Algorithm_Type::UNIFSIGMOID}};

class AlgorithmFactory {
public:
  static std::shared_ptr<Algorithm> make(std::string algorithm_name,
                                         Algorithm::Parameters algo_params,
                                         const nlohmann::json &params);
};

} // namespace maxcut
