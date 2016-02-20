#include "nfaEdge.h"
#include <string>
#include "stringTools.h"

using namespace rgx;
using namespace std;

/*=========================_NFA_Edge=================================================*/

rgx::_NFA_Edge::_NFA_Edge(const visitor_ptr<_NFA_Node> &toNode) 
    : _toNode(toNode) {}

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

string rgx::_epsilonEdge::toString() {
    string info;
    info += "\n epsilonEdge \n";
    return info;
}


/*===================================charSetEdge=======================================*/

rgx::_charSetEdge::_charSetEdge(const visitor_ptr<_NFA_Node> &toNode, set<unsigned int> &&s, unsigned int delopt) 
    : _NFA_Edge(toNode), _acceptSet(s), _delOPT(delopt) {}

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

string rgx::_loopStartEdge::toString() {
    string info;
    info += "\n loopStartEdge \n";
    info += " loopTimes : " + int2string(_lowerLoopTimes) + "  " + int2string(_upperLoopTimes) + "\n";
    info += " greedy : " + bool2string(_greedy) + "\n";
    return info;
}

/*===================================loopEndEdge==========================================*/

rgx::_loopEndEdge::_loopEndEdge(const visitor_ptr<_NFA_Node> &toNode, int llt, int ult, bool grd)
    : _NFA_Edge(toNode), _lowerLoopTimes(llt), _upperLoopTimes(ult), _greedy(grd) {}

string rgx::_loopEndEdge::toString() {
    string info;
    info += "\n loopEndEdge \n";
    info += " loopTimes : " + int2string(_lowerLoopTimes) + "  " + int2string(_upperLoopTimes) + "\n";
    info += " greedy : " + bool2string(_greedy) + "\n";
    return info;
}


/*================================captureStart===========================================*/

rgx::_captureStartEdge::_captureStartEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int captureindex) 
    : _NFA_Edge(toNode), _captureIndex(captureindex) {}

string rgx::_captureStartEdge::toString() {
    string info;
    info += "\n captureStartEdge\n";
    return info;
}


/*==============================captureEndEdge===========================================*/

rgx::_captureEndEdge::_captureEndEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int captureindex) 
    : _NFA_Edge(toNode), _captureIndex(captureindex) {}

string rgx::_captureEndEdge::toString() {
    string info;
    info += "\n captureEndEdge \n";
    info += " captureIndex : " + int2string(_captureIndex) + "\n";
    return info;
}



/*===============================referenceEdge============================================*/

rgx::_referenceEdge::_referenceEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int referenceindex) 
    :_NFA_Edge(toNode), _referenceIndex(referenceindex) {}

string rgx::_referenceEdge::toString() {
    string info;
    info += "\n referenceEdge\n";
    info += " referenceIndex: " + int2string(_referenceIndex) + "\n\n";
    return info;
}


/*================================positionEdge===========================================*/

rgx::_positionEdge::_positionEdge(const visitor_ptr<_NFA_Node> &toNode, position_type position) 
    : _NFA_Edge(toNode), _position(position) {}

string rgx::_positionEdge::toString() {
    string info;
    info += "\n positionEdge \n";
    info += " position_type : " + int2string(_position) + "\n\n";
    return info;
}
