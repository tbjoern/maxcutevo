#pragma once
#include "types.hpp"

namespace maxcut {

class Algorithm {
private:
  /**
   * initializes the tracking of the cut state, e.g. _change, _part and
   * _cut_weight
   */
  void initCutTracking();

protected:
  /**
   *  _change[k] says how much weight will be added or subtracted from the
   * cut_weight if the node is flipped. _part keeps track of each nodes part (1
   * or -1)
   */
  std::vector<int> _change, _part;
  // total current weight of the cut
  int _cut_weight, _max_cut_weight;
  const AdjList *_adj_list;
  int _node_count;

  /**
   * In the MaxCut problem, the graph is divided into two parts. All edges that
   * have endpoints in both parts are part of the cut. Flipping a node means
   * moving it to the opposite part.
   */
  void flipNode(int nodeID);

  void flipNodes(std::vector<int> nodeIDs);

  bool flipNodesIfBetterCut(std::vector<int> nodeIDs);

  /**
   * Implement MaxCut solving algorithms by overriding this method
   */
  virtual void run() = 0;

public:
  Algorithm() : _change(), _part(), _cut_weight(0), _node_count(0){};
  /**
   * runs the algorithm contained in Algorithm::run
   * runs initCutTracking before run
   * @returns the maximum found cut weight
   */
  int calcSolution(const AdjList &);

  virtual std::string name() = 0;
};

} // namespace maxcut
