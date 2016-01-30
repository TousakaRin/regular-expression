#ifndef _F_A_NODE_
#define _F_A_NODE_

#include <vector>
#include <memory>

namespace rgx {

/******************前置声明*********************/

class _NFA_Edge;
class epsilonEdge;
class charSetEdge;
class loopStartEdge;
class loopEndEdge;
class captureStartEdge;
class captureEndEdge;
class referenceEdge;
class positionEdge;

/******************前置声明*********************/
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
    std::shared_ptr<referenceEdge> addReferenceEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<positionEdge> addPositionEdge(std::shared_ptr<_NFA_Node>);
};

}
#endif
