#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "astnode.h"

namespace rgx {


class _ast {
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
    std::shared_ptr<_astNode> pre_read_term();
    std::shared_ptr<_astNode> post_read_term();
    std::shared_ptr<_astNode> charSet_term();
    std::shared_ptr<_numCount_node> num_term();
    void err();

    bool isKeyword(std::wstring::size_type);
    int getNum();
private:
    static wchar_t _cat_start_marsk[8];
    static wchar_t _keyword[17];
    std::vector<std::vector<std::wstring> > catchArray;
    std::map<std::wstring, unsigned int> nameMap; 
};

}

#endif
