#ifndef _F_A_NODE_
#define _F_A_NODE_

#include <vector>
#include "nfaEdge.h"

namespace rgx {

class _NFA_Node {
public: 
    _NFA_Node();
    std::vector<std::shared_ptr<_NFA_Edge>> edges;
    std::shared_ptr<epsilonEdge> addEpsilonEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<charSetEdge> addCharSetEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<loopStartEdge> addLoopStartEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<loopEndEdge> addLoopEndEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<captureStartEdge> addCaptureStartEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<captureEndEdge> addCaptureEndEdge(std::shared_ptr<_NFA_Node>);
};

}
#endif
