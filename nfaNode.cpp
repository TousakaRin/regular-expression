#include "nfaNode.h"
#include "pattern.h"
#include "nfaEdge.h"
#include "astnode.h"
#include "ast.h"

using namespace std;
using namespace rgx;

rgx::_NFA_Node::_NFA_Node() {
}

void rgx::_NFA_Node::err() {
    exit(-1);
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

void rgx::_NFA_Node::addCharSetEdge(const visitor_ptr<_NFA_Node> &goalNode, const _charSet_node& csn) {
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


void rgx::_NFA_Node::addLoopStartEdge(const visitor_ptr<_NFA_Node> &goalNode, const _numCount_node& ncn) {
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _loopStartEdge(goalNode, ncn._lowerLoopTimes, ncn._upperLoopTimes, ncn._greedy)));
}



void rgx::_NFA_Node::addLoopEndEdge(const visitor_ptr<_NFA_Node> &goalNode, const _numCount_node& ncn) {
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _loopEndEdge(goalNode, ncn._lowerLoopTimes, ncn._upperLoopTimes, ncn._greedy)));
}


void rgx::_NFA_Node::addCaptureStartEdge(const visitor_ptr<_NFA_Node> &goalNode, const _capture_node &cn) {
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _captureStartEdge(goalNode, cn._captureIndex)));
}


void rgx::_NFA_Node::addCaptureEndEdge(const visitor_ptr<_NFA_Node> &goalNode, const _capture_node &cn) {
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _captureEndEdge(goalNode, cn._captureIndex)));
}

void rgx::_NFA_Node::addReferenceEdge(const visitor_ptr<_NFA_Node> &goalNode, const _reference_node &refn) {
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _referenceEdge(goalNode, refn._referenceIndex)));
}

void rgx::_NFA_Node::addPositionEdge(const visitor_ptr<_NFA_Node> &goalNode, const _position_node &psn) {
    edges.push_back(unique_ptr<_NFA_Edge>(
                new _positionEdge(goalNode, psn._position)));
}
