#include "nfaEdge.h"
#include <string>
#include "stringTools.h"

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

