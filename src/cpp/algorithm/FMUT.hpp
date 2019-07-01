#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"

namespace maxcut {

class FMUT : public Algorithm {
  double power_law_beta;
  PowerLawGenerator plaw_gen;

public:
  FMUT(Algorithm::Parameters params, double power_law_beta);

  void iteration() override;
};

} // namespace maxcut
