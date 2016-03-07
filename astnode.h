#ifndef _AST_NODE_H_
#define _AST_NODE_H_

#include <string>
#include <memory>
#include <vector>
#include "visitor_ptr.h"
#include "edgeManager.h"
#include "stringTools.h"
#include "nfaNode.h"
#include "typedef.h"


namespace rgx {

class _NFA_Node;
class _pattern;

/*-----------------------------------------------*/

class _numCount_node;
class _preRead_node;
class _astNode {
public:
    _astNode();
    visitor_ptr<_astNode> _left, _right;
    _NFA_Ptr _NFAptr;                         //加上它是为了在生成NFA时更方便的使用非递归算法
    void err(const std::string&);
    void err();
    virtual std::string toString();
    virtual void generateNFA(_pattern&);      //永远不应该调用这个方法，此处不作为纯虚函数，仅用于方便调试
    virtual ~_astNode();  
};




/*-----------------------------------------------*/
class _or_node : public _astNode {
public:
    virtual std::string toString();
    virtual void generateNFA(_pattern&);
};




/*-----------------------------------------------*/
class _charSet_node: public _astNode {
public:    
    _charSet_node(char16_t);
    _charSet_node(std::shared_ptr<_edgeManager>);
    //每一个pair表示一个范围
    //范围内的每一个字符由'|'连接
    //如pair{a, c + 1} === [a-c] === (?:a|b|c)
    //deleteOPT 表示\W, \S, \D三个集合
    enum deleteOPT {NO_WORD = 0x1, NO_DIGIT = 0x10, NO_SPACE = 0x100};
    unsigned int _delOPT;
    std::shared_ptr<_edgeManager> _edgeMgr;
    std::vector<std::pair<char16_t, char16_t>> _acceptSet;
    void addCharRange(const std::pair<char16_t, char16_t>&);

    void addWordRange();
    void addUWordRange();

    void addDigitRange();
    void addUDigitRange();

    void addSpaceRang();
    void addUSpaceRange();

    void setInversison();
    bool inversion;

    virtual std::string toString();
    virtual void generateNFA(_pattern&);
};




/*-----------------------------------------------*/
class _cat_node : public _astNode {
public:
    virtual std::string toString();
    virtual void generateNFA(_pattern&);
};





/*-----------------------------------------------*/
class _preRead_node {
//正向预读和反向预读使用同一种节点表示即可
public:
    _preRead_node(bool);
    _preRead_node(const visitor_ptr<_astNode>&, bool);
    bool pattern_tag; //预读内容是否匹配,即(?:<!) 还是(?:<=)
    visitor_ptr<_astNode> _dfaTree;
    std::string toString();
    void generateDFA(_pattern&);
};





/*-----------------------------------------------*/

class _capture_node : public _astNode {
public:    
    unsigned int _captureIndex;
    std::u16string _captureName;
    virtual std::string toString();
    virtual void generateNFA(_pattern&);
};




/*-----------------------------------------------*/
class _reference_node : public _astNode {
public:
    std::u16string _referenceName;
    unsigned int _referenceIndex;
    _reference_node(unsigned int);
    _reference_node(unsigned int, const std::u16string&);
    virtual std::string toString();
    virtual void generateNFA(_pattern&);
};



/*-----------------------------------------------*/


class _numCount_node : public _astNode {
public:
    _numCount_node();
    _numCount_node(int, int);
    _numCount_node(int, int, bool);
    bool _greedy;
    int _lowerLoopTimes, _upperLoopTimes;
    virtual std::string toString();
    std::unique_ptr<_preRead_node> _pre_read, _post_read;
    virtual void generateNFA(_pattern&);
};


/*-----------------------------------------------*/


class _position_node : public _astNode {
public:
    position_type _position;
    _position_node(position_type);
    virtual std::string toString();
    std::string positionString();
    virtual void generateNFA(_pattern&);
};


}
#endif

