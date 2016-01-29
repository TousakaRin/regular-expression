#ifndef _AST_NODE_H_
#define _AST_NODE_H_

#include <string>
#include <memory>
#include <vector>
#include "nfaNode.h"
#include "edgeManager.h"
#include "dfaTable.h"


namespace rgx {

typedef std::shared_ptr<std::pair<std::shared_ptr<_NFA_Node>, std::shared_ptr<_NFA_Node>>>  _NFA_ptr;
typedef std::pair<std::shared_ptr<_NFA_Node>, std::shared_ptr<_NFA_Node>> _NFA;
typedef std::shared_ptr<_DFA_Table>  _DFA_ptr;

/*-----------------------------------------------*/
class _numCount_node;
class _preRead_node;
class _astNode {
public:
    std::shared_ptr<_astNode> left, right;
    _NFA_ptr leftNFA, rightNFA;          //加上它是为了在生成NFA时更方便的使用非递归算法
    virtual std::string toString();
    virtual _NFA_ptr generateNFA();      //永远不应该调用这个方法，此处不作为纯虚函数，仅用于方便调试
    virtual ~_astNode();  
};




/*-----------------------------------------------*/
class _or_node : public _astNode {
public:
    virtual std::string toString();
    virtual _NFA_ptr generateNFA();
};




/*-----------------------------------------------*/
class _charSet_node: public _astNode {
public:    
    _charSet_node(char16_t);
    _charSet_node();
    //每一个pair表示一个范围
    //范围内的每一个字符由'|'连接
    //如pair{a, c + 1} === [a-c] === (?:a|b|c)
    //deleteOPT 表示\W, \S, \D三个集合
    enum deleteOPT {NO_WORD = 0x1, NO_DIGIT = 0x10, NO_SPACE = 0x100};
    unsigned int delOPT;
    std::vector<std::pair<char16_t, char16_t>> charset;
    void addCharRange(const std::pair<char16_t, char16_t>&, std::shared_ptr<edgeManager>);

    void addWordRange(std::shared_ptr<edgeManager>);
    void addUWordRange();

    void addDigitRange(std::shared_ptr<edgeManager>);
    void addUDigitRange();

    void addSpaceRang(std::shared_ptr<edgeManager>);
    void addUSpaceRange();

    void setInversison();
    bool inversion;

    virtual std::string toString();
    virtual _NFA_ptr generateNFA();
};




/*-----------------------------------------------*/
class _cat_node : public _astNode {
public:
    virtual std::string toString();
    virtual _NFA_ptr generateNFA();
};





/*-----------------------------------------------*/
class _preRead_node {
//正向预读和反向预读使用同一种节点表示即可
public:
    _preRead_node(bool);
    _preRead_node(std::shared_ptr<_astNode>, bool);
    bool pattern_tag; //预读内容是否匹配,即(?:<!) 还是(?:<=)
    std::shared_ptr<_astNode> dfaTree;
    std::string toString();
    _DFA_ptr generateDFA();
};





/*-----------------------------------------------*/

class _capture_node : public _astNode {
public:    
    unsigned int captureIndex;
    std::u16string name;
    virtual std::string toString();
    virtual _NFA_ptr generateNFA();
};




/*-----------------------------------------------*/
class _reference_node : public _astNode {
public:
    std::u16string name;
    unsigned int index;
    _reference_node(unsigned int);
    _reference_node(unsigned int, const std::u16string&);
    virtual std::string toString();
    virtual _NFA_ptr generateNFA();
};



/*-----------------------------------------------*/


class _numCount_node : public _astNode {
public:
    _numCount_node();
    _numCount_node(int, int);
    _numCount_node(int, int, bool);
    bool greedy;
    int lower, upper;
    virtual std::string toString();
    std::shared_ptr<_preRead_node> pre_read, post_read;
    virtual _NFA_ptr generateNFA();
};


/*-----------------------------------------------*/


class _position_node : public _astNode {
public:
    enum position_type {LINE_BEGIN, LINE_END, STRING_BEGIN, STRING_END, BREAK_OFF, NO_BREAK_OFF} position;
    _position_node(position_type);
    virtual std::string toString();
    std::string positionString();
    virtual _NFA_ptr generateNFA();
};


}
#endif
