#include "nfaNode.h"

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

shared_ptr<charSetEdge> rgx::_NFA_Node::addCharSetEdge(std::shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<charSetEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}


shared_ptr<loopStartEdge> rgx::_NFA_Node::addLoopStartEdge(std::shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<loopStartEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}



shared_ptr<loopEndEdge> rgx::_NFA_Node::addLoopEndEdge(std::shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<loopEndEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}



shared_ptr<captureStartEdge> rgx::_NFA_Node::addCaptureStartEdge(std::shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<captureStartEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}


shared_ptr<captureEndEdge> rgx::_NFA_Node::addCaptureEndEdge(std::shared_ptr<_NFA_Node> goalNode) {
    auto newEdge = make_shared<captureEndEdge>();
    newEdge->toNode = goalNode;
    edges.push_back(newEdge);
    return newEdge;
}
