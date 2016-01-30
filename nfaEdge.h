#ifndef _NFA_EDGE_
#define _NFA_EDGE_

#include <memory>
#include <vector>
#include "astnode.h"

namespace  rgx {

class _NFA_Node;


//NFA 边的父类
class _NFA_Edge {
public:
    std::shared_ptr<_NFA_Node> toNode;
    virtual std::string toString();
};


//epsilon边，即空边
class epsilonEdge : public _NFA_Edge {
public:
    virtual std::string toString();
};


//接受一个字符类的边，只能接受一个字符
class charSetEdge : public _NFA_Edge {
public:
    std::vector<unsigned int> acceptSet;
    unsigned int delOPT;
    virtual std::string toString();

};

//循环开始边
class loopStartEdge : public _NFA_Edge {
public:
    virtual std::string toString();
};

//循环结束边
class loopEndEdge : public _NFA_Edge {
public:
    virtual std::string toString();
    int lowerLoopTimes, upperLoopTimes;
    bool greedy;
};

//表示开始捕获
class captureStartEdge : public _NFA_Edge {
public:
    unsigned int referenceIndex;
    virtual std::string toString();
};

//表示捕获结束,保存捕获结果
class captureEndEdge : public _NFA_Edge {
public:
    unsigned int referenceIndex;
    virtual std::string toString();
};

//表示引用边
class referenceEdge : public _NFA_Edge {
public:
    unsigned int referenceIndex;
    virtual std::string toString();
};


//表示位置边，占位符，如$ ^
class positionEdge : public _NFA_Edge {
public:
    _position_node::position_type position;
    virtual std::string toString();
};

}
#endif
