#ifndef _F_A_NODE_
#define _F_A_NODE_

#include <vector>
#include <memory>
#include <iostream>
#include <set>
#include <utility>
#include "visitor_ptr.h"
#include "nfaEdge.h"

namespace rgx {

/******************前置声明*********************/

class _pattern;
template<typename T>
class visitor_ptr;
class _charSet_node;
class _position_node;
class _reference_node;
class _numCount_node;
class _capture_node;

/******************前置声明*********************/
class _NFA_Node {
public: 
    _NFA_Node();
    std::vector<std::unique_ptr<_NFA_Edge>> edges;
    void addEpsilonEdge(const visitor_ptr<_NFA_Node>&);
    void addCharSetEdge(visitor_ptr<_NFA_Node>&, const _charSet_node&);
    void addLoopStartEdge(visitor_ptr<_NFA_Node>&, const _numCount_node&);
    void addLoopEndEdge(visitor_ptr<_NFA_Node>&, const _numCount_node&);
    void addCaptureStartEdge(visitor_ptr<_NFA_Node>&, const _capture_node&);
    void addCaptureEndEdge(visitor_ptr<_NFA_Node>&, const _capture_node&);
    void addReferenceEdge(visitor_ptr<_NFA_Node>&, const _reference_node&);
    void addPositionEdge(visitor_ptr<_NFA_Node>&, const _position_node&);


//    std::vector<visitor_ptr<_NFA_Node>> nonEpsilonEdgeVec();         //返回所有非空边的集合
    void deleteEpsilonEdge();
    void err();
    void err(const std::string&);
    bool _effective;
    void setEffective();
};

typedef std::pair<visitor_ptr<_NFA_Node>, visitor_ptr<_NFA_Node>> _NFA;
typedef std::unique_ptr<_NFA> _NFA_Ptr;

}
#endif

