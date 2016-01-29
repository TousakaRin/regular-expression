#ifndef _F_A_NODE_
#define _F_A_NODE_

#include <vector>
#include "fa_edge.h"

namespace rgx {

class _FA_Node {

};

class _DFA_Node : public _FA_Node {
};

class _NFA_Node : public _FA_Node {
public: 
    _NFA_Node();
    std::vector<_NFA_Edge> edges;
};

}
#endif
