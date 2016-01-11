#ifndef _AST_NODE_H_
#define _AST_NODE_H_

#include <string>
#include <memory>
#include <vector>

namespace rgx {

/*-----------------------------------------------*/
class _numCount_node;
class _astNode {
public:
    std::shared_ptr<_astNode> pre_read, post_read;
    std::shared_ptr<_astNode> left, right;
    std::shared_ptr<_numCount_node> numCount;
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
    _charSet_node(wchar_t);
    _charSet_node();
    //每一个pair表示一个范围
    //范围内的每一个字符由'|'连接
    //如pair{a, c} === [a-c] === (?:a|b|c)
    std::vector<std::pair<wchar_t, wchar_t>> charset;
    void addCharRange(const std::pair<wchar_t, wchar_t>&);
    void addCharRange(std::pair<wchar_t, wchar_t>&&);
    void addDeleteRange(std::pair<wchar_t, wchar_t>&&);
    void addDeleteRange(const std::pair<wchar_t, wchar_t>&);

    void addWordRange();
    void deleteWordRange();

    void addDigitRange();
    void delteDigitRange();

    void addSpaceRang();
    void deleteSpaceRange();

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
class _preRead_node : public _astNode {
//正向预读和反向预读使用同一种节点表示即可
public:
    _preRead_node(bool);
    _preRead_node(std::shared_ptr<_astNode>, bool);
    bool pattern_tag; //预读内容是否匹配,即(?:<!) 还是(?:<=)
    std::shared_ptr<_astNode> dfaTree;
    virtual std::string toString();
};





/*-----------------------------------------------*/
class _catch_node : public _astNode {
public:    
    unsigned int catchIndex;
    std::wstring name;
    virtual std::string toString();
};





/*-----------------------------------------------*/
class _reference_node : public _astNode {
public:
    std::wstring name;
    unsigned int index;
    _reference_node(unsigned int);
    _reference_node(unsigned int, const std::wstring&);
    virtual std::string toString();
};





/*-----------------------------------------------*/


class _numCount_node {
public:
    _numCount_node();
    _numCount_node(int, int);
    _numCount_node(int, int, bool);
    bool greedy;
    int lower, upper;
    virtual std::string toString();
};




}
#endif
