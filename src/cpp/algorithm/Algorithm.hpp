#pragma once
#include "../MathHelper.hpp"
#include "../types.hpp"

#include <nlohmann/json.hpp>

#include <csignal>

namespace maxcut {

/**
 * for every node that is part of the CUT_SET the weight of the outgoing edges
 * that have an endpoint in the NOT_CUT_SET is counted towards the cut size.
 */
enum side { CUT_SET = -1, NOT_CUT_SET = 1 };

class Algorithm {
protected:
  /**
   * initializes the tracking of the cut state, e.g. _change, _part and
   * _cut_weight
   * resets helper variables to default
   */

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

  void flipNodes(std::vector<int> &nodeIDs);

  bool flipNodesIfBetterCut(std::vector<int> &nodeIDs);

  int changeByFlip(int nodeID);

  virtual void init() = 0;

  virtual void iteration() = 0;

public:
  Algorithm() : _part(), _cut_weight(0), _max_cut_weight(0), _node_count(0){};
  virtual ~Algorithm(){};

  virtual std::string name() = 0;

  virtual void _iteration();

  int getCutSize();

  Cut calcCutSizes();

  virtual void _init();

  void setGraph(const AdjList &adj_list);

  int evaluation_count;

  int id;

  virtual void parse_arguments(nlohmann::json) {}
};

} // namespace maxcut