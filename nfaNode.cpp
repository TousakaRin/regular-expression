#include "nfaNode.h"
#include "pattern.h"
#include "nfaEdge.h"
#include "astnode.h"
#include "ast.h"

using namespace std;
using namespace rgx;

rgx::_NFA_Node::_NFA_Node() : _effective(false) {
}

void rgx::_NFA_Node::err() {
    exit(-1);
}

//vector<visitor_ptr<_NFA_Node>> rgx::_NFA_Node::nonEpsilonEdgeVec() {
//    vector<visitor_ptr<_NFA_Node>> neev;
//    for (unsigned int i = 0; i < edges.size(); ++i) {
//        if (!edges[i]->isEpsilonEdge()) {
//            neev.push_back(edges[i]->_toNode);
//        }
//    }
//    return neev;
//}


void rgx::_NFA_Node::deleteEpsilonEdge() {
    vector<unique_ptr<_NFA_Edge>> noEpsilonEdges;    
    for (auto &ptr : edges) {
        if(!ptr->isEpsilonEdge()) {
            noEpsilonEdges.push_back(std::move(ptr));
        }
    }
    swap(noEpsilonEdges, edges);
}

void rgx::_NFA_Node::err(const string& msg) {
    cout << "\n--------------------------------------" << endl
        << msg 
        << "\n-----------------------------------------" << endl;
    err();
}

void rgx::_NFA_Node::addEpsilonEdge(const visitor_ptr<_NFA_Node> &goalNode) {
    edges.push_back(unique_ptr<_NFA_Edge>(new _epsilonEdge(goalNode)));
}

void rgx::_NFA_Node::addCharSetEdge(visitor_ptr<_NFA_Node> &goalNode, const _charSet_node& csn) {
    goalNode->setEffective();
    set<unsigned int> acceptSet;
    if (auto p = csn._edgeMgr.lock()) {
        for (auto range : csn._acceptSet) {
            unsigned int pre = p->_hashTable[range.first];
            acceptSet.insert(pre);
            for (auto i = range.first; i <  range.second; ++i) {
                if (p->_hashTable[i] != pre) {
                    acceptSet.insert(pre);
                    pre = p->_hashTable[i];
                }
            }
        }
        unique_ptr<_charSetEdge> newEdge(new _charSetEdge(goalNode, std::move(acceptSet), csn._delOPT));
        edges.push_back(std::move(newEdge));
    } else {
        err("function addCharSetEdge : 找不到对象 ");
    }
}


void rgx::_NFA_Node::setEffective() {
    _effective = true;    
}


void rgx::_NFA_Node::addLoopStartEdge(visitor_ptr<_NFA_Node> &goalNode, const _numCount_node& ncn) {
    goalNode->setEffective();
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _loopStartEdge(goalNode, ncn._lowerLoopTimes, ncn._upperLoopTimes, ncn._greedy)));
}


void rgx::_NFA_Node::addLoopEndEdge(visitor_ptr<_NFA_Node> &goalNode, const _numCount_node& ncn) {
    goalNode->setEffective();
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _loopEndEdge(goalNode, ncn._lowerLoopTimes, ncn._upperLoopTimes, ncn._greedy)));
}


void rgx::_NFA_Node::addCaptureStartEdge(visitor_ptr<_NFA_Node> &goalNode, const _capture_node &cn) {
    goalNode->setEffective();
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _captureStartEdge(goalNode, cn._captureIndex)));
}


void rgx::_NFA_Node::addCaptureEndEdge(visitor_ptr<_NFA_Node> &goalNode, const _capture_node &cn) {
    goalNode->setEffective();
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _captureEndEdge(goalNode, cn._captureIndex)));
}

void rgx::_NFA_Node::addReferenceEdge(visitor_ptr<_NFA_Node> &goalNode, const _reference_node &refn) {
    goalNode->setEffective();
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _referenceEdge(goalNode, refn._referenceIndex)));
}

void rgx::_NFA_Node::addPositionEdge(visitor_ptr<_NFA_Node> &goalNode, const _position_node &psn) {
    goalNode->setEffective();
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _positionEdge(goalNode, psn._position)));
}

