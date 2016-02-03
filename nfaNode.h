#ifndef _F_A_NODE_
#define _F_A_NODE_

#include <vector>
#include <memory>

namespace rgx {

/******************前置声明*********************/

class _NFA_Edge;
class _epsilonEdge;
class _charSetEdge;
class _loopStartEdge;
class _loopEndEdge;
class _captureStartEdge;
class _captureEndEdge;
class _referenceEdge;
class _positionEdge;

/******************前置声明*********************/
class _NFA_Node {
public: 
    _NFA_Node();
    std::vector<std::shared_ptr<_NFA_Edge>> edges;
    std::shared_ptr<_epsilonEdge> addEpsilonEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<_charSetEdge> addCharSetEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<_loopStartEdge> addLoopStartEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<_loopEndEdge> addLoopEndEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<_captureStartEdge> addCaptureStartEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<_captureEndEdge> addCaptureEndEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<_referenceEdge> addReferenceEdge(std::shared_ptr<_NFA_Node>);
    std::shared_ptr<_positionEdge> addPositionEdge(std::shared_ptr<_NFA_Node>);
};

}
#endif
