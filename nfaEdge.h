#ifndef _NFA_EDGE_
#define _NFA_EDGE_

#include <memory>
#include <set>
#include <iostream>
#include <vector>
#include "visitor_ptr.h"
#include "typedef.h"

namespace  rgx {

class _NFA_Node;
class _position_node;


//NFA 边的父类
class _NFA_Edge {
public:
    _NFA_Edge(const visitor_ptr<_NFA_Node> &toNode);
    visitor_ptr<_NFA_Node> _toNode;
    virtual std::string toString();
};


//epsilon边，即空边
class _epsilonEdge : public _NFA_Edge {
public:
    _epsilonEdge(const visitor_ptr<_NFA_Node> &toNode);
    virtual std::string toString();
};


//接受一个字符类的边，只能接受一个字符
class _charSetEdge : public _NFA_Edge {
public:
    _charSetEdge(const visitor_ptr<_NFA_Node> &toNode, std::set<unsigned int>&& s, unsigned int delopt);
    std::set<unsigned int> _acceptSet;
    unsigned int _delOPT;
    virtual std::string toString();

};

//循环开始边
class _loopStartEdge : public _NFA_Edge {
public:
    _loopStartEdge(const visitor_ptr<_NFA_Node> &toNode, int llt, int ult, bool grd);
    virtual std::string toString();
    int _lowerLoopTimes, _upperLoopTimes;
    bool _greedy;
};

//循环结束边
class _loopEndEdge : public _NFA_Edge {
public:
    _loopEndEdge(const visitor_ptr<_NFA_Node> &toNode, int llt, int ult, bool grd);
    virtual std::string toString();
    int _lowerLoopTimes, _upperLoopTimes;
    bool _greedy;
};

//表示开始捕获
class _captureStartEdge : public _NFA_Edge {
public:
    _captureStartEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int captureindex);
    unsigned int _captureIndex;
    virtual std::string toString();
};

//表示捕获结束,保存捕获结果
class _captureEndEdge : public _NFA_Edge {
public:
    _captureEndEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int captureindex);
    unsigned int _captureIndex;
    virtual std::string toString();
};

//表示引用边
class _referenceEdge : public _NFA_Edge {
public:
    _referenceEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int referenceindex);
    unsigned int _referenceIndex;
    virtual std::string toString();
};


//表示位置边，占位符，如$ ^
class _positionEdge : public _NFA_Edge {
public:
    _positionEdge(const visitor_ptr<_NFA_Node> &toNode, position_type);
    position_type _position;
    virtual std::string toString();
};

}
#endif
