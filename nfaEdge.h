#ifndef _NFA_EDGE_
#define _NFA_EDGE_

#include <memory>
#include <set>
#include <iostream>
#include <stack>
#include <vector>
#include "visitor_ptr.h"
#include "edgeManager.h"
#include "typedef.h"

namespace  rgx {

class _NFA_Node;
class _position_node;
class _thread;
class matchObj;


//NFA 边的父类
class _NFA_Edge {
public:
    _NFA_Edge(const visitor_ptr<_NFA_Node> &toNode);
    _NFA_Edge(const _NFA_Edge&);

    visitor_ptr<_NFA_Node> _toNode;
    virtual bool isEpsilonEdge() const = 0;
    virtual std::string toString();
    virtual _NFA_Edge* makeCopy() const = 0;
    virtual int match(const std::u16string&, _thread&, std::stack<_thread>&) = 0;
    virtual bool lookahead(const std::u16string&, unsigned int);
	virtual ~_NFA_Edge() = default;
};


//epsilon边，即空边
class _epsilonEdge : public _NFA_Edge {
public:
    _epsilonEdge(const _epsilonEdge&);
    _epsilonEdge(const visitor_ptr<_NFA_Node> &toNode);

    virtual std::string toString();
    bool isEpsilonEdge() const { return true; }
    virtual _epsilonEdge* makeCopy() const;
    virtual int match(const std::u16string&, _thread&, std::stack<_thread>&);
};


//接受一个字符类的边，只能接受一个字符
class _charSetEdge : public _NFA_Edge {
public:
    _charSetEdge(const _charSetEdge&);
    _charSetEdge(const visitor_ptr<_NFA_Node> &toNode, std::set<unsigned int>&& s, const std::shared_ptr<_edgeManager>&, unsigned int delopt, bool ivs);

    std::set<unsigned int> _acceptSet;
    unsigned int _delOPT;
    virtual std::string toString();
    bool isEpsilonEdge() const { return false; }
    virtual _charSetEdge* makeCopy() const;
    virtual int match(const std::u16string&, _thread&, std::stack<_thread>&);
    std::shared_ptr<_edgeManager> _edgeMgr;
    virtual bool lookahead(const std::u16string&, unsigned int);
    bool inversion;
};

//循环开始边
class _loopStartEdge : public _NFA_Edge {
public:
    _loopStartEdge(const _loopStartEdge&);
    _loopStartEdge(const visitor_ptr<_NFA_Node> &toNode, const visitor_ptr<_NFA_Node> &loopEndNode, int llt, int ult, bool grd);

    virtual std::string toString();
    int _lowerLoopTimes, _upperLoopTimes;
    bool _greedy;
    visitor_ptr<_NFA_Node> _loopEndNode;

    virtual _loopStartEdge* makeCopy() const;
    bool isEpsilonEdge() const { return false; }
    virtual int match(const std::u16string&, _thread&, std::stack<_thread>&);
};

//循环结束边
class _loopEndEdge : public _NFA_Edge {
public:
    _loopEndEdge(const _loopEndEdge&);
    _loopEndEdge(const visitor_ptr<_NFA_Node> &toNode, const visitor_ptr<_NFA_Node>& loopStartNode, int llt, int ult, bool grd);

    virtual std::string toString();
    virtual _loopEndEdge* makeCopy() const;
    int _lowerLoopTimes, _upperLoopTimes;
    bool _greedy;
    visitor_ptr<_NFA_Node> _loopStartNode;
    bool isEpsilonEdge() const { return false; }
    virtual int match(const std::u16string&, _thread&, std::stack<_thread>& );
};

//表示开始捕获
class _captureStartEdge : public _NFA_Edge {
public:
    _captureStartEdge(const _captureStartEdge&);
    _captureStartEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int captureindex);
    unsigned int _captureIndex;
    virtual std::string toString();
    virtual _captureStartEdge* makeCopy() const;
    bool isEpsilonEdge() const { return false; }
    virtual int match(const std::u16string&, _thread&, std::stack<_thread>&);
    bool lookahead(const std::u16string&, unsigned int);
};

//表示捕获结束,保存捕获结果
class _captureEndEdge : public _NFA_Edge {
public:
    _captureEndEdge(const _captureEndEdge&);
    _captureEndEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int captureindex);
    unsigned int _captureIndex;
    virtual std::string toString();
    virtual _captureEndEdge* makeCopy() const;
    bool isEpsilonEdge() const { return false; }
    virtual int match(const std::u16string&, _thread&, std::stack<_thread>&);
    bool lookahead(const std::u16string&, unsigned int);
};

//表示引用边
class _referenceEdge : public _NFA_Edge {
public:
    _referenceEdge(const _referenceEdge&);
    _referenceEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int referenceindex);
    unsigned int _referenceIndex;
    virtual  _referenceEdge* makeCopy() const;
    virtual std::string toString();
    bool isEpsilonEdge() const { return false; }
    virtual int match(const std::u16string&, _thread&, std::stack<_thread>&);
};


//表示位置边，占位符，如$ ^
class _positionEdge : public _NFA_Edge {
public:
    _positionEdge(const _positionEdge&);
    _positionEdge(const visitor_ptr<_NFA_Node> &toNode, position_type);
    position_type _position;
    virtual std::string toString();
    virtual _positionEdge* makeCopy() const;
    bool isEpsilonEdge() const { return false; }
    bool isWordBreak(const std::u16string&, unsigned int);
    virtual int match(const std::u16string&, _thread&, std::stack<_thread>&);
};


}
#endif

