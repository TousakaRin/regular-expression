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
    std::shared_ptr<_astNode> root;
    std::wstring::size_type pos;
    std::shared_ptr<_astNode> or_term(const std::wstring&);
    std::shared_ptr<_astNode> re_term(const std::wstring&);
    std::shared_ptr<_astNode> cat_term(const std::wstring&);
    std::shared_ptr<_astNode> pre_read_term(const std::wstring&);
    std::shared_ptr<_astNode> post_read_term(const std::wstring&);
    std::shared_ptr<_astNode> charSet_term(const std::wstring&);
    void err(std::wstring::size_type, const std::wstring&);

private:
    static wchar_t _cat_start_marsk[8];
    std::vector<std::vector<std::wstring> > catchArray;
    std::map<std::wstring, unsigned int> nameMap; 
};

}

#endif
