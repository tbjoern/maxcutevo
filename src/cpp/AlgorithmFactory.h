
#include "Algorithm.h"
#include "types.h"

namespace maxcut {

class AlgorithmFactory {
public:
    virtual Algorithm build(AdjList& adj_list) = 0;
};

}
