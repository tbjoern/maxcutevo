#include "Runner.hpp"

#include <chrono>
#include <csignal>
#include <functional>
#include <unistd.h>

namespace maxcut {

std::function<void(int)> interrupt_handler;
void signal_handler(int signal) { interrupt_handler(signal); }

RunResult Runner::runAlgorithm(const AdjList &adj_list, Algorithm &algorithm) {
  interrupt_handler = [&algorithm](int) { algorithm.stop = 1; };

  std::signal(SIGALRM, signal_handler);
  alarm(Runner::ALGORITHM_RUN_DURATION);

  auto start_time = std::chrono::high_resolution_clock::now();

  int cut_size = algorithm.calcSolution(adj_list);

  auto stop_time = std::chrono::high_resolution_clock::now();
  double total_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                          stop_time - start_time)
                          .count();

  return {algorithm.name(), cut_size, total_time};
}

} // namespace maxcut
