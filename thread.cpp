#include "thread.h"
#include "nfaNode.h"

using namespace std;
using namespace rgx;

rgx::_thread::_thread(const visitor_ptr<_NFA_Node> &pc, unsigned int sp, unsigned int edgeIndex) 
    :  _sp(sp), _nodePtr(pc), _edgeIndex(edgeIndex) { 
}

int rgx::_thread::match(const u16string& input, stack<_thread> &threadstack, const unique_ptr<matchObj>& storgePtr, const shared_ptr<_edgeManager> &edgeMgr) {
    while (_nodePtr->edges.size() != 0) {
    }
    return 0;
}

