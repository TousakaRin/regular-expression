#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include "visitor_ptr.h"
#include "objectPool.h"
#include "edgeManager.h"

namespace rgx {

/*************************前置声明*****************************/

class _ast;
void astTraversal(const _ast&);
class _astNode;
class _or_node;
class _charSet_node;
class _cat_node;
class _capture_node;
class _preRead_node;
class _reference_node;
class _numCount_node;
class _position_node;

/*************************前置声明*****************************/

class _ast {
    friend void rgx::astTraversal(const _ast&);
    friend class _pattern;

public:
    enum {build_to_dfa, build_to_nfa} _build_type;                                 //转换标记，表示该ast可以转换成NFA还是DFA
    _ast(const std::string &regular_expression);
    _ast(const _ast&) = delete;
    ~_ast();

private:
    visitor_ptr<_astNode> re_term();                                               //处理整个正则xxx或括号中的子句
    visitor_ptr<_astNode> or_term();                                               //处理'|'语法
    visitor_ptr<_astNode> cat_term();                                              //处理连接语法
    std::unique_ptr<_preRead_node> pre_read_term();                                //正向预读
    std::unique_ptr<_preRead_node> post_read_term();                               //反向预读
    visitor_ptr<_position_node> position_term();                                   //位置节点 \A \Z \b \B ^ $
    visitor_ptr<_astNode> charSet_term();                                          //字符或者(xx)
    visitor_ptr<_astNode> normalBracket();                                         //普通括号    (?:)
    visitor_ptr<_capture_node> namedCapture();                                     //具名捕获    (?P<name>)
    visitor_ptr<_capture_node> unnamedCapture();                                   //匿名捕获    ()
    visitor_ptr<_reference_node> namedReference(const std::u16string &);           //具名引用    (?P=name)
    visitor_ptr<_reference_node> unnamedReference();                               //匿名引用    \index
    visitor_ptr<_astNode> normalTrans();                                           //常规转义    \s,\?,\+
    visitor_ptr<_charSet_node> charClass();                                        //字符类      [^xxx] | [xxx]
    visitor_ptr<_numCount_node> num_term();                                        //数量单位    ?, ??, +, +?....
    bool charSetTrans(visitor_ptr<_charSet_node>&);                                //处理字符类中的预定义字符类 /w,/W,/s,/S,/d,/D
    
    void err();                                                                    //出错处理
    int getNum();                                                                  //从pos位置获取一个非负整数，失败时返回-1
private:
    const std::u16string _re;                                                      //正则表达式
    _objectPool<_astNode> _objPool;
    visitor_ptr<_astNode> _root;                                                   //ast的root
    std::u16string::size_type _pos;                                                //lookahead位置
    std::map<std::u16string, unsigned int> _nameMap;                               //具名捕获-->捕获index的转换
    unsigned int _captureIndex;                                                    //当前捕获分组的个数，初始为0，用于捕获匹配串，每增加一个用户捕获时，自增1
    std::shared_ptr<_edgeManager> _edgeMgr;                                        //自动机边数量压缩转换表
    static char16_t _cat_start_mask[8];                                            //不能出现在cat_term开头的字符
    static char16_t _normalTrans_set[20];                                          //能够出现在转义字符后面的关键字(在字符类中的转义字符另外处理)
    static char16_t _charSet_mask[6];                                              //不能出现在charSet_term的关键字集合
};

}

#endif
