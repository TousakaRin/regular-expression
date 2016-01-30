#include "nfaNode.h"
#include "nfaEdge.h"

using namespace std;
using namespace rgx;

rgx::_NFA_Node::_NFA_Node() {

}

shared_ptr<epsilonEdge>rgx::_NFA_Node::addEpsilonEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<epsilonEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}

shared_ptr<charSetEdge> rgx::_NFA_Node::addCharSetEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<charSetEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}


shared_ptr<loopStartEdge> rgx::_NFA_Node::addLoopStartEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<loopStartEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}



shared_ptr<loopEndEdge> rgx::_NFA_Node::addLoopEndEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<loopEndEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}


shared_ptr<captureStartEdge> rgx::_NFA_Node::addCaptureStartEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<captureStartEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}


shared_ptr<captureEndEdge> rgx::_NFA_Node::addCaptureEndEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<captureEndEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}

shared_ptr<referenceEdge> rgx::_NFA_Node::addReferenceEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<referenceEdge>();
    newEdge->toNode  = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}

shared_ptr<positionEdge> rgx::_NFA_Node::addPositionEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<positionEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}
