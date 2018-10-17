#pragma once
#include "MathHelper.hpp"
#include "types.hpp"

#include <csignal>

namespace maxcut {

/**
 * for every node that is part of the CUT_SET the weight of the outgoing edges
 * that have an endpoint in the NOT_CUT_SET is counted towards the cut size.
 */
enum side { CUT_SET = -1, NOT_CUT_SET = 1 };

class Algorithm {
private:
  /**
   * initializes the tracking of the cut state, e.g. _change, _part and
   * _cut_weight
   * resets helper variables to default
   */
  void init();

protected:
  void calculateChange();

  std::pair<int, int> calculateCurrentCutSize();
  /**
   *  _change[k] says how much weight will be added or subtracted from the
   * cut_weight if the node is flipped. _part keeps track of each nodes part (1
   * or -1)
   */
  std::vector<int> _part, _change;
  // total current weight of the cut
  int _cut_weight, _max_cut_weight;
  int _node_count;
  const AdjList *_adj_list;
  MathHelper helper;

  /**
   * In the MaxCut problem, the graph is divided into two parts. All edges that
   * have endpoints in both parts are part of the cut. Flipping a node means
   * moving it to the opposite part.
   */
  void flipNode(int nodeID);

  void flipNodes(std::vector<int> nodeIDs);

  bool flipNodesIfBetterCut(std::vector<int> nodeIDs);

  int changeByFlip(int nodeID);

  /**
   * Implement MaxCut solving algorithms by overriding this method
   */
  virtual void run() = 0;

  Cut calcCutSizes();

public:
  std::sig_atomic_t volatile stop = 0;

  Algorithm() : _part(), _cut_weight(0), _max_cut_weight(0), _node_count(0){};
  virtual ~Algorithm(){};
  /**
   * runs the algorithm contained in Algorithm::run
   * runs initCutTracking before run
   * @returns the maximum found cut weight
   */
  Cut calcSolution(const AdjList &);

  virtual std::string name() = 0;
};

} // namespace maxcut
