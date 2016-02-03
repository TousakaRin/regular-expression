#ifndef _NFA_EDGE_
#define _NFA_EDGE_

#include <memory>
#include <set>
#include <vector>
#include "astnode.h"

namespace  rgx {

class _NFA_Node;


//NFA 边的父类
class _NFA_Edge {
public:
    std::shared_ptr<_NFA_Node> _toNode;
    virtual std::string toString();
};


//epsilon边，即空边
class _epsilonEdge : public _NFA_Edge {
public:
    virtual std::string toString();
};


//接受一个字符类的边，只能接受一个字符
class _charSetEdge : public _NFA_Edge {
public:
    std::set<unsigned int> _acceptSet;
    unsigned int _delOPT;
    virtual std::string toString();

};

//循环开始边
class _loopStartEdge : public _NFA_Edge {
public:
    virtual std::string toString();
    int _lowerLoopTimes, _upperLoopTimes;
    bool _greedy;
};

//循环结束边
class _loopEndEdge : public _NFA_Edge {
public:
    virtual std::string toString();
    int _lowerLoopTimes, _upperLoopTimes;
    bool _greedy;
};

//表示开始捕获
class _captureStartEdge : public _NFA_Edge {
public:
    unsigned int _captureIndex;
    virtual std::string toString();
};

//表示捕获结束,保存捕获结果
class _captureEndEdge : public _NFA_Edge {
public:
    unsigned int _captureIndex;
    virtual std::string toString();
};

//表示引用边
class _referenceEdge : public _NFA_Edge {
public:
    unsigned int _referenceIndex;
    virtual std::string toString();
};


//表示位置边，占位符，如$ ^
class _positionEdge : public _NFA_Edge {
public:
    _position_node::position_type _position;
    virtual std::string toString();
};

}
#endif
