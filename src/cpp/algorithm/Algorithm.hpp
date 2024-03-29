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
  std::vector<char> _part;
  std::vector<int> _change;
  // total current weight of the cut
  int _cut_weight, _max_cut_weight;
  int _node_count;
  int _nodes_flipped;
  const AdjList &_adj_list;
  MathHelper helper;

  /**
   * In the MaxCut problem, the graph is divided into two parts. All edges that
   * have endpoints in both parts are part of the cut. Flipping a node means
   * moving it to the opposite part.
   */
  void flipNode(int nodeID);

  void flipNodes(std::vector<int> &nodeIDs);

  bool flipNodesIfBetterCut(std::vector<int> &nodeIDs);

  bool flipNodesIfAboveThreshold(std::vector<int> &nodeIDs,
                                 double threshold_factor);

  int changeByFlip(int nodeID);

public:
  struct Parameters {
    const AdjList &adj_list;
    const bool use_start_assignment;
    const std::vector<char> &start_assignment;
  };
  // Algorithm() : {};
  Algorithm(Parameters params);
  virtual ~Algorithm(){};

  virtual void iteration() = 0;

  virtual std::vector<NodeInfo> getNodeInfo();

  int getCutSize();

  Cut calcCutSizes();

  void setGraph(const AdjList &adj_list);

  virtual void parse_arguments(nlohmann::json) {}

  int getNodesFlipped() const;

  void clear_stats();
};

} // namespace maxcut
