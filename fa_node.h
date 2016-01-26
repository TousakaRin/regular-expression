#ifndef _F_A_NODE_
#define _F_A_NODE_

namespace rgx {

class _FA_Node {

};

class _DFA_Node : public _FA_Node {

};

class _NFA_Node : public _FA_Node {
public:
    bool finishNode;
};

}
#endif
