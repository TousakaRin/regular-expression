#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "astnode.h"

namespace rgx {

class _ast;
void astTraversal(const _ast&);

class _ast {
    friend void rgx::astTraversal(const _ast&);
public:
    enum {build_to_dfa, build_to_nfa} build_type;
    _ast(const std::wstring &regular_expression);
    _ast(const _ast&) = delete;
    ~_ast();
private:
    const std::wstring re;
    std::shared_ptr<_astNode> root;
    std::wstring::size_type pos;
    std::shared_ptr<_astNode> or_term();
    std::shared_ptr<_astNode> re_term();
    std::shared_ptr<_astNode> cat_term();
    std::shared_ptr<_preRead_node> pre_read_term();
    std::shared_ptr<_preRead_node> post_read_term();
    std::shared_ptr<_astNode> charSet_term();             
    std::shared_ptr<_astNode> normalBracket();                        //普通括号    (?:)
    std::shared_ptr<_astNode> namedCatch();                           //具名捕获    (?P<name>)
    std::shared_ptr<_astNode> unnamedCatch();                         //匿名捕获    ()
    std::shared_ptr<_astNode> namedReference(const std::wstring &);   //具名引用    (?P=name)
    std::shared_ptr<_astNode> unnamedReference();         //匿名引用    \index
    std::shared_ptr<_astNode> normalTrans();                          //常规转义    \s,\?,\+
    std::shared_ptr<_astNode> charClass();                            //字符类      [^xxx] | [xxx]
    std::shared_ptr<_numCount_node> num_term();                       //数量单位    ?, ??, +, +?....
    bool charSetTrans(std::shared_ptr<_charSet_node>);                //处理字符类中的预定义字符类 /w,/W,/s,/S,/d,/D
    void err();

    bool isKeyword(std::wstring::size_type);
    int getNum();
private:
    static wchar_t _cat_start_mask[8];    //不能出现在cat_term开头的字符
    static wchar_t _normalTrans_set[18];    //能够出现在转义字符后面的关键字(在字符类中的转义字符另外处理)
    static wchar_t _charSet_mask[6];

    std::map<std::wstring, unsigned int> nameMap; 
    unsigned int catchNum;
};

}

#endif
