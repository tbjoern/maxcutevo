#include "Batch.hpp"
#include "Runner.hpp"
#include <csignal>
#include <functional>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>

namespace maxcut {

std::function<void(int)> interrupt_handler;
void signal_handler(int signal) { interrupt_handler(signal); }

std::mutex mutex;

void runner_thread(const AdjList &adj_list,
                   std::shared_ptr<Algorithm> algorithm,
                   std::vector<RunResult> &results) {
  auto result = Runner::runAlgorithm(adj_list, *algorithm);
  mutex.lock();
  results.push_back(result);
  mutex.unlock();
}

void test() {}
Batch::Batch(AdjList &&adj_list,
             std::vector<std::shared_ptr<Algorithm>> &algorithms)
    : _adj_list(adj_list), _algorithms(algorithms) {}

std::vector<RunResult> Batch::run() const {

  std::vector<RunResult> results;
  std::vector<std::thread> threads;
  for (auto &algorithm : _algorithms) {
    threads.emplace_back(runner_thread, std::ref(_adj_list), algorithm,
                         std::ref(results));
  }

  interrupt_handler = [&](int) {
    for (auto &algorithm : _algorithms) {
      algorithm->stop = 1;
    }
  };

  std::signal(SIGALRM, signal_handler);
  alarm(Batch::ALGORITHM_RUN_DURATION);

  for (auto &thread : threads) {
    thread.join();
  }

  return results;
}

} // namespace maxcut
