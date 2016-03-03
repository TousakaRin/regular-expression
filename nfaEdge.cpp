#include "nfaEdge.h"
#include <string>
#include "stringTools.h"
#include "thread.h"
#include "nfaNode.h"

using namespace rgx;
using namespace std;

/*=========================_NFA_Edge=================================================*/

rgx::_NFA_Edge::_NFA_Edge(const visitor_ptr<_NFA_Node> &toNode) 
    : _toNode(toNode) {}

rgx::_NFA_Edge::_NFA_Edge(const _NFA_Edge& ne) 
    : _toNode(ne._toNode) {}

string rgx::_NFA_Edge::toString() {
    string info;
    info += "\n _NFA_Node \n";
    info += " an error occured \n";
    info += " this function should never be called \n\n";
    return info;
}

/*================================epsilonEdge==========================================*/

rgx::_epsilonEdge::_epsilonEdge(const visitor_ptr<_NFA_Node> &toNode) 
    : _NFA_Edge(toNode) {}

rgx::_epsilonEdge::_epsilonEdge(const _epsilonEdge& ee) 
    : _NFA_Edge(ee) {}

_epsilonEdge* rgx::_epsilonEdge::makeCopy() const {
    return new _epsilonEdge(*this);   
}

string rgx::_epsilonEdge::toString() {
    string info;
    info += "\n epsilonEdge \n";
    return info;
}


int rgx::_epsilonEdge::match(const std::u16string&, _thread&, std::stack<_thread>&, const std::unique_ptr<matchObj>&, const std::shared_ptr<_edgeManager>&) {
    cout << "err, nfa中怎么可能会有epsilon" << endl;
    return -2;
}

/*===================================charSetEdge=======================================*/

rgx::_charSetEdge::_charSetEdge(const visitor_ptr<_NFA_Node> &toNode, set<unsigned int> &&s, unsigned int delopt) 
    : _NFA_Edge(toNode), _acceptSet(s), _delOPT(delopt) {}


_charSetEdge *rgx::_charSetEdge::makeCopy() const {
    return new _charSetEdge(*this);    
}

rgx::_charSetEdge::_charSetEdge(const _charSetEdge& cse) : _NFA_Edge(cse._toNode), _acceptSet(cse._acceptSet), _delOPT(cse._delOPT) {

}

string rgx::_charSetEdge::toString() {
    string info;
    info += "\n charSetEdge \n ";
    for (auto i : _acceptSet) {
        info += int2string(i) + " ";
    }
    info += "\n delete_OPT " + int2string(_delOPT);
    info += "\n";
    return info;
}


int rgx::_charSetEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>& , const std::unique_ptr<matchObj>&, const std::shared_ptr<_edgeManager>& edgeMgr) {
    if (thread._sp >= input.size() || _acceptSet.find(edgeMgr->getIndex(input[thread._sp])) == _acceptSet.end()) {
        return -1;
    } else {
        thread._nodePtr = thread._nodePtr->edges[thread._edgeIndex]->_toNode;
        thread._edgeIndex = 0;
        ++thread._sp;
        return 0;
    }
}

/*================================loopStartEdge===========================================*/

rgx::_loopStartEdge::_loopStartEdge(const visitor_ptr<_NFA_Node> &toNode, int llt, int ult, bool grd) 
    : _NFA_Edge(toNode), _lowerLoopTimes(llt), _upperLoopTimes(ult), _greedy(grd) {}


rgx::_loopStartEdge::_loopStartEdge(const _loopStartEdge& lse) : _NFA_Edge(lse._toNode), _lowerLoopTimes(lse._lowerLoopTimes), _upperLoopTimes(lse._upperLoopTimes), _greedy(lse._greedy) {
    
}


string rgx::_loopStartEdge::toString() {
    string info;
    info += "\n loopStartEdge \n";
    info += " loopTimes : " + int2string(_lowerLoopTimes) + "  " + int2string(_upperLoopTimes) + "\n";
    info += " greedy : " + bool2string(_greedy) + "\n";
    return info;
}

_loopStartEdge* rgx::_loopStartEdge::makeCopy() const {
    return new _loopStartEdge(*this);
}

int rgx::_loopStartEdge::match(const std::u16string&, _thread& thread, std::stack<_thread>& threadstack, const std::unique_ptr<matchObj>&, const shared_ptr<_edgeManager>&) {
    return 0;
}


/*===================================loopEndEdge==========================================*/

rgx::_loopEndEdge::_loopEndEdge(const visitor_ptr<_NFA_Node> &toNode, int llt, int ult, bool grd)
    : _NFA_Edge(toNode), _lowerLoopTimes(llt), _upperLoopTimes(ult), _greedy(grd) {}


rgx::_loopEndEdge::_loopEndEdge(const _loopEndEdge& lee) : _NFA_Edge(lee._toNode), _lowerLoopTimes(lee._lowerLoopTimes), _upperLoopTimes(lee._upperLoopTimes), _greedy(lee._greedy) {

}


string rgx::_loopEndEdge::toString() {
    string info;
    info += "\n loopEndEdge \n";
    info += " loopTimes : " + int2string(_lowerLoopTimes) + "  " + int2string(_upperLoopTimes) + "\n";
    info += " greedy : " + bool2string(_greedy) + "\n";
    return info;
}

_loopEndEdge* rgx::_loopEndEdge::makeCopy() const {
    return new  _loopEndEdge(*this);
}

int rgx::_loopEndEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>& threadstack, const std::unique_ptr<matchObj>& storgePtr, const shared_ptr<_edgeManager>&) {
    return 0;
}

/*================================captureStart===========================================*/

rgx::_captureStartEdge::_captureStartEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int captureindex) 
    : _NFA_Edge(toNode), _captureIndex(captureindex) {}


rgx::_captureStartEdge::_captureStartEdge(const _captureStartEdge& cse) : _NFA_Edge(cse._toNode), _captureIndex(cse._captureIndex) {

}


string rgx::_captureStartEdge::toString() {
    string info;
    info += "\n captureStartEdge\n";
    return info;
}

_captureStartEdge* rgx::_captureStartEdge::makeCopy() const {
    return new _captureStartEdge(*this);
}

int rgx::_captureStartEdge::match(const std::u16string&, _thread& thread, std::stack<_thread>&, const std::unique_ptr<matchObj>& storgePtr, const shared_ptr<_edgeManager>&) {
    storgePtr->_capVector[_captureIndex].first = thread._sp;
    thread._nodePtr = thread._nodePtr->edges[thread._edgeIndex]->_toNode;
    thread._edgeIndex = 0;
    return 0;
}

/*==============================captureEndEdge===========================================*/

rgx::_captureEndEdge::_captureEndEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int captureindex) 
    : _NFA_Edge(toNode), _captureIndex(captureindex) {}

rgx::_captureEndEdge::_captureEndEdge(const _captureEndEdge& cee) : _NFA_Edge(cee._toNode), _captureIndex(cee._captureIndex) {
    
}


string rgx::_captureEndEdge::toString() {
    string info;
    info += "\n captureEndEdge \n";
    info += " captureIndex : " + int2string(_captureIndex) + "\n";
    return info;
}

_captureEndEdge* rgx::_captureEndEdge::makeCopy() const {
    return new _captureEndEdge(*this);
}

int rgx::_captureEndEdge::match(const std::u16string&, _thread& thread, std::stack<_thread>&, const std::unique_ptr<matchObj>& storgePtr, const shared_ptr<_edgeManager>&) {
    storgePtr->_capVector[_captureIndex].second = thread._sp;
    thread._nodePtr = thread._nodePtr->edges[thread._edgeIndex]->_toNode;
    thread._edgeIndex = 0;
    return 0;
}

/*===============================referenceEdge============================================*/

rgx::_referenceEdge::_referenceEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int referenceindex) 
    :_NFA_Edge(toNode), _referenceIndex(referenceindex) {}

rgx::_referenceEdge::_referenceEdge(const _referenceEdge& re) : _NFA_Edge(re._toNode), _referenceIndex(re._referenceIndex) {

}


string rgx::_referenceEdge::toString() {
    string info;
    info += "\n referenceEdge\n";
    info += " referenceIndex: " + int2string(_referenceIndex) + "\n\n";
    return info;
}

_referenceEdge* rgx::_referenceEdge::makeCopy() const {
    return new _referenceEdge(*this);
}
int rgx::_referenceEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>&, const std::unique_ptr<matchObj>& storgePtr, const shared_ptr<_edgeManager>&) {
    for (unsigned int i = storgePtr->_capVector[_referenceIndex].first; i < storgePtr->_capVector[_referenceIndex].second; ) {
        if (input[i] == input[thread._sp]) {
            ++thread._sp;
            ++i;
        } else {
            return -1;
        }
    }
    thread._nodePtr = thread._nodePtr->edges[thread._edgeIndex]->_toNode;
    thread._edgeIndex = 0;
    return 0;
}

/*================================positionEdge===========================================*/

rgx::_positionEdge::_positionEdge(const visitor_ptr<_NFA_Node> &toNode, position_type position) 
    : _NFA_Edge(toNode), _position(position) {}

rgx::_positionEdge::_positionEdge(const _positionEdge &pse) : _NFA_Edge(pse._toNode), _position(pse._position) {
    
}


string rgx::_positionEdge::toString() {
    string info;
    info += "\n positionEdge \n";
    info += " position_type : " + int2string(_position) + "\n\n";
    return info;
}

_positionEdge* rgx::_positionEdge::makeCopy() const {
    return new _positionEdge(*this);
}

int rgx::_positionEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>& , const std::unique_ptr<matchObj>& , const shared_ptr<_edgeManager>&) {
    switch (_position) {
        case LINE_BEGIN :
            if (thread._sp == 0 || input[thread._sp - 1] == '\n') {
                break; 
            } else {
                return -1;
            }
        case LINE_END :
            if (thread._sp == input.size() || input[thread._sp - 1] == '\n') {
                break;
            } else {
                return -1;
            }
        case STRING_BEGIN :
            if (thread._sp == 0) {
                break;
            } else {
                return -1;
            }
        case STRING_END :
            if (thread._sp == input.size()) {
                break;
            } else {
                return -1;
            }
        case BREAK_OFF :
            if (isWordBreak(input, thread._sp)) {
                break;
            } else {
                return -1;
            }
        case NO_BREAK_OFF :
            if (!isWordBreak(input, thread._sp)) {
                break;
            } else {
                return -1;
            }
        default : 
            return -1;
    } 
    thread._nodePtr = thread._nodePtr->edges[thread._edgeIndex]->_toNode;
    thread._edgeIndex = 0;
    return 0;
}

bool rgx::_positionEdge::isWordBreak(const u16string& input, unsigned int index) {
    if (index == 0 || index == input.size()) {
        return false;
    } else {
        if ((isDigit(input[index]) || isLetter(input[index])) && (!isDigit(input[index - 1]) && !isLetter(index - 1))) {
            return true;
        } else if ((isDigit(input[index - 1]) || isLetter(input[index - 1])) && (!isDigit(input[index]) && !isLetter(input[index]))) {
            return true;
        } else {
            return false;
        }
    }
}

