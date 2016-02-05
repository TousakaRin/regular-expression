#include "nfaNode.h"
#include "pattern.h"
#include "nfaEdge.h"
#include "ast.h"

using namespace std;
using namespace rgx;

rgx::_NFA_Node::_NFA_Node(_pattern& p) {
    id = p._NFA_nodeCount++;
}

shared_ptr<_epsilonEdge>rgx::_NFA_Node::addEpsilonEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<_epsilonEdge>();
    newEdge->_toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}

shared_ptr<_charSetEdge> rgx::_NFA_Node::addCharSetEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<_charSetEdge>();
    newEdge->_toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}


shared_ptr<_loopStartEdge> rgx::_NFA_Node::addLoopStartEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<_loopStartEdge>();
    newEdge->_toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}



shared_ptr<_loopEndEdge> rgx::_NFA_Node::addLoopEndEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<_loopEndEdge>();
    newEdge->_toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}


shared_ptr<_captureStartEdge> rgx::_NFA_Node::addCaptureStartEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<_captureStartEdge>();
    newEdge->_toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}


shared_ptr<_captureEndEdge> rgx::_NFA_Node::addCaptureEndEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<_captureEndEdge>();
    newEdge->_toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}

shared_ptr<_referenceEdge> rgx::_NFA_Node::addReferenceEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<_referenceEdge>();
    newEdge->_toNode  = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}

shared_ptr<_positionEdge> rgx::_NFA_Node::addPositionEdge(shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<_positionEdge>();
    newEdge->_toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}
