#include "nfaEdge.h"
#include <string>
#include "stringTools.h"

using namespace rgx;
using namespace std;

/*=========================_NFA_Edge=================================================*/

string rgx::_NFA_Edge::toString() {
    string info;
    info += "\n _NFA_Node \n";
    info += " an error occured \n";
    info += " this function should never be called \n\n";
    return info;
}

/*================================epsilonEdge==========================================*/

string rgx::_epsilonEdge::toString() {
    string info;
    info += "\n epsilonEdge \n";
    return info;
}


/*===================================charSetEdge=======================================*/

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

string rgx::_loopStartEdge::toString() {
    string info;
    info += "\n loopStartEdge \n";
    info += " loopTimes : " + int2string(_lowerLoopTimes) + "  " + int2string(_upperLoopTimes) + "\n";
    info += " greedy : " + bool2string(_greedy) + "\n";
    return info;
}

/*===================================loopEndEdge==========================================*/

string rgx::_loopEndEdge::toString() {
    string info;
    info += "\n loopEndEdge \n";
    info += " loopTimes : " + int2string(_lowerLoopTimes) + "  " + int2string(_upperLoopTimes) + "\n";
    info += " greedy : " + bool2string(_greedy) + "\n";
    return info;
}


/*================================captureStart===========================================*/

string rgx::_captureStartEdge::toString() {
    string info;
    info += "\n captureStartEdge\n";
    return info;
}


/*==============================captureEndEdge===========================================*/

string rgx::_captureEndEdge::toString() {
    string info;
    info += "\n captureEndEdge \n";
    info += " captureIndex : " + int2string(_captureIndex) + "\n";
    return info;
}



/*===============================referenceEdge============================================*/

string rgx::_referenceEdge::toString() {
    string info;
    info += "\n referenceEdge\n";
    info += " referenceIndex: " + int2string(_referenceIndex) + "\n\n";
    return info;
}


/*================================epsilonEdge===========================================*/

string rgx::_positionEdge::toString() {
    string info;
    info += "\n positionEdge \n";
    info += " position_type : " + int2string(_position) + "\n\n";
    return info;
}
