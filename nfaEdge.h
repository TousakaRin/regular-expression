#ifndef _NFA_EDGE_
#define _NFA_EDGE_

#include <memory>

namespace  rgx {

class _NFA_Node;


//NFA 边的父类
class _NFA_Edge {
public:
    std::shared_ptr<_NFA_Node> toNode;
};


//epsilon边，即空边
class epsilonEdge : public _NFA_Edge {

};


//接受一个字符类的边，只能接受一个字符
class charSetEdge : public _NFA_Edge {

};

//循环开始边
class loopStartEdge : public _NFA_Edge {

};

//循环结束边
class loopEndEdge : public _NFA_Edge {
    
};

//表示开始捕获
class captureStartEdge : public _NFA_Edge {

};

//表示捕获结束,保存捕获结果
class captureEndEdge : public _NFA_Edge {

};

}
#endif
