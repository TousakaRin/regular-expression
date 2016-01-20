#ifndef _AST_NODE_H_
#define _AST_NODE_H_

#include <string>
#include <memory>
#include <vector>
#include "edgeManager.h"

namespace rgx {

/*-----------------------------------------------*/
class _numCount_node;
class _preRead_node;
class _astNode {
public:
    std::shared_ptr<_astNode> left, right;
    virtual std::string toString();
    virtual ~_astNode();  
};




/*-----------------------------------------------*/
class _or_node : public _astNode {
public:
    virtual std::string toString();
};




/*-----------------------------------------------*/
class _charSet_node: public _astNode {
public:    
    _charSet_node(char16_t);
    _charSet_node();
    //每一个pair表示一个范围
    //范围内的每一个字符由'|'连接
    //如pair{a, c} === [a-c] === (?:a|b|c)
    std::vector<std::pair<char16_t, char16_t>> charset;
    std::vector<std::pair<char16_t, char16_t>> deletedCharset;
    void addCharRange(const std::pair<char16_t, char16_t>&, std::shared_ptr<edgeManager>);
    void addCharRange(std::pair<char16_t, char16_t>&&, std::shared_ptr<edgeManager>);
    void addDeleteRange(std::pair<char16_t, char16_t>&&, std::shared_ptr<edgeManager>);
    void addDeleteRange(const std::pair<char16_t, char16_t>&, std::shared_ptr<edgeManager>);

    void addWordRange(std::shared_ptr<edgeManager>);
    void deleteWordRange(std::shared_ptr<edgeManager>);

    void addDigitRange(std::shared_ptr<edgeManager>);
    void delteDigitRange(std::shared_ptr<edgeManager>);

    void addSpaceRang(std::shared_ptr<edgeManager>);
    void deleteSpaceRange(std::shared_ptr<edgeManager>);

    void setInversison();
    bool inversion;

    virtual std::string toString();
};




/*-----------------------------------------------*/
class _cat_node : public _astNode {
public:
    virtual std::string toString();
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
};





/*-----------------------------------------------*/
class _catch_node : public _astNode {
public:    
    unsigned int catchIndex;
    std::u16string name;
    virtual std::string toString();
};





/*-----------------------------------------------*/
class _reference_node : public _astNode {
public:
    std::u16string name;
    unsigned int index;
    _reference_node(unsigned int);
    _reference_node(unsigned int, const std::u16string&);
    virtual std::string toString();
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
};


/*-----------------------------------------------*/


class _position_node : public _astNode {
public:
    enum position_type {LINE_BEGIN, LINE_END, STRING_BEGIN, STRING_END, BREAK_OFF, NO_BREAK_OFF} position;
    _position_node(position_type);
    virtual std::string toString();
    std::string positionString();
};


}
#endif
