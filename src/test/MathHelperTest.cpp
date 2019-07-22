#include <MathHelper.hpp>
#include <iostream>

using namespace maxcut;

const int node_count = 100;
const auto samples = node_count * 100;

void test_probability_sampling() {
  MathHelper helper;

  auto probability = 1.0 / node_count;

  auto sampler = helper.probabilitySampler(probability);
  for (int i = 0; i < samples; ++i) {
    std::cout << sampler->get() << std::endl;
  }
}

void test_powerlaw() {
  MathHelper helper;

  PowerLawGenerator gen{node_count, 3.5, helper.getEngine()};

  for (int i = 0; i < samples; ++i) {
    std::cout << gen.get() << std::endl;
  }
}

int main() { test_powerlaw(); }
