#include "nfaEdge.h"
#include <string>
#include "stringTools.h"

using namespace rgx;
using namespace std;

/*=========================_NFA_Edge=================================================*/

string rgx::_NFA_Edge::toString() {
    string info;
    info += "\n\n _NFA_Node \n";
    info += " an error occured \n";
    info += " this function should never be called \n\n";
    return info;
}

/*================================epsilonEdge==========================================*/

string rgx::epsilonEdge::toString() {
    string info;
    info += "\n\n epsilonEdge \n\n";
    return info;
}


/*===================================charSetEdge=======================================*/

string rgx::charSetEdge::toString() {
    string info;
    info += "\n\n charSetEdge \n";
    for (auto i : acceptSet) {
        info += int2string(i) + " ";
    }
    info += "\n delete_OPT" + int2string(delOPT);
    info += "\n\n";
    return info;
}

/*================================loopStartEdge===========================================*/

string rgx::loopStartEdge::toString() {
    string info;
    info += "\n\n loopStartEdge \n\n";
    return info;
}

/*===================================loopEndEdge==========================================*/

string rgx::loopEndEdge::toString() {
    string info;
    info += "\n\n loopEndEdge \n";
    info += " loopTimes : " + int2string(lowerLoopTimes) + "  " + int2string(upperLoopTimes) + "\n";
    info += " greedy : " + bool2string(greedy) + "\n";
    return info;
}


/*================================captureStart===========================================*/

string rgx::captureStartEdge::toString() {
    string info;
    info += "\n\n captureStartEdge\n";
    return info;
}


/*==============================captureEndEdge===========================================*/

string rgx::captureEndEdge::toString() {
    string info;
    info += "\n\n captureStartEdge \n";
    info += " captureIndex : " + int2string(referenceIndex) + "\n";
    return info;
}



/*===============================referenceEdge============================================*/

string rgx::referenceEdge::toString() {
    string info;
    info += "\n\n captureEndEdge \n";
    info += " captureIndex : " + int2string(referenceIndex) + "\n\n";
    return info;
}


/*================================epsilonEdge===========================================*/

string rgx::positionEdge::toString() {
    string info;
    info += "\n\n positionEdge \n";
    info += " position_type : " + int2string(position) + "\n\n";
    return info;
}
