#include "UnifSigmoid.hpp"
#include <unordered_map>

using namespace std;

namespace maxcut {

std::function<bool(int)> UnifSigmoid::build_activity_sigmoid_sampler() {
  unordered_map<int, BernoulliGenerator> act_to_sampler;
  double max_p = 1.0 / 2, center_p = 1.0 / _node_count,
         min_p = 1.0 / (_node_count * _node_count);
  double upper_multiplier = max_p - center_p;
  double lower_multiplier = center_p - min_p;
  for (int activity = parameters.ACT_MIN; activity <= parameters.ACT_MAX;
       ++activity) {
    double sigmoid_value = helper.sigmoid(activity, sigmoid_smoothness);
    double probability = 0.0;
    if (sigmoid_value > 1) {
      sigmoid_value -= 1;
      probability = upper_multiplier * sigmoid_value + center_p;
    } else {
      probability = lower_multiplier * sigmoid_value + min_p;
    }
    act_to_sampler[activity] = helper.probabilitySampler(probability);
  }
  return [act_to_sampler](double activity) mutable {
    return act_to_sampler.at(static_cast<int>(ceil(activity))).get();
  };
}

UnifSigmoid::UnifSigmoid(Algorithm::Parameters params,
                         ActivityAlgorithm::Parameters parameters,
                         double sigmoid_smoothness)
    : ActivityAlgorithm(params, parameters) {
  this->sigmoid_smoothness = sigmoid_smoothness;
  activity_sampler = build_activity_sigmoid_sampler();
}

void UnifSigmoid::iteration() {
  vector<int> nodes_to_flip;
  for (int node = 0; node < _node_count; ++node) {
    if (activity_sampler(_activity[node])) {
      nodes_to_flip.push_back(node);
    }
  }
  bool flipped = flipNodesIfBetterCut(nodes_to_flip);

  if (flipped) {
    updateActivity(nodes_to_flip);
    decayActivity();
  }
}

} // namespace maxcut
