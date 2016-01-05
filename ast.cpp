#include "ast.h"
#include <cassert>

using namespace std;
using namespace rgx;

void _ast::err(wstring::size_type pos, const wstring&re) {
    cout << "语法错误哟~" << endl;
    cout << "position :  " << pos << endl;
    decltype(pos) start = pos > 5 ? pos - 5 : 0, end = pos + 5 <= re.size() ? pos + 5 : re.size();
    wcout << re.substr(start, end - start) << endl;
}

_ast::_ast(const wstring &re) : pos(0) {
    root = re_term(re);
    if (pos != re.size()) {
        root = nullptr;
        err(pos, re);
    }
}

shared_ptr<_astNode> _ast::re_term(const wstring &re) {
    if (pos >= re.size()) {
        err(pos, re);
        return nullptr;
    } 
    auto r = or_term(re);         //r 始终为子树的根节点
    while (pos < re.size() && re[pos] == '|') {
        ++pos;           // match '|'
        if (pos >= re.size()) {
            err(pos, re);
            return nullptr;
        }
        auto n = or_term(re);
        if (n) {
            auto newRoot = make_shared<_or_node>();
            newRoot->left = r;
            newRoot->right = n;
            r = newRoot;
        } else {
            err(pos, re);
            return nullptr;
        }
    }
    return r;
}

shared_ptr<_astNode> _ast::or_term(const wstring& re) {
    if (pos >= re.size()) {
        err(pos, re);
        return nullptr;
    }
    auto r = cat_term(re); 
    while (pos < re.size()) {
        for (auto c : _cat_start_marsk) {
            if (re[pos] == c) {
                return r;
            }
        }
        auto n = cat_term(re);
        if (n) {
            auto newRoot = make_shared<_cat_node>();
            newRoot->left = r;
            newRoot->right = n;
            r = newRoot;
        } else {
            err(pos, re);
            return nullptr;
        }
    } 
    return r;
}

shared_ptr<_astNode> _ast::cat_term(const wstring& re) {
    // cat_term 可以为null
    if (pos >= re.size()) {
        return nullptr;
    }
    auto pre = pre_read_term(re);
    auto r = charSet_term(re);
    auto post = post_read_term(re);
    if (r) {
        r->pre_read = pre; 
        r->post_read = post;
    }
    return r;
}

shared_ptr<_astNode> _ast::pre_read_term(const wstring &re) {
    //pre_read_term 可以为nullptr
    if (pos + 5 >= re.size()) {  //至少6个字符，如: (?<!x)
        return nullptr;
    } else if (re[pos] == '(' && re[pos + 1] == '?' && re[pos + 2] == '<' && (re[pos + 3] == '!' || re[pos + 3] == '=')) {
        auto n = re_term(re);
        if (n && pos < re.size() && re[pos] == ')') {
            auto r = make_shared<_preRead_node>();
            r->left = n;
            r->flag = re[pos + 3] == '!';
            return r;
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

shared_ptr<_astNode> _ast::post_read_term(const wstring &re) {
    //pre_read_term 可以为nullptr
    if (pos + 4 >= re.size()) {  //至少6个字符，如: (?<!x)
        return nullptr;
    } else if (re[pos] == '(' && re[pos + 1] == '?'  && (re[pos + 2] == '!' || re[pos + 2] == '=')) {
        auto n = re_term(re);
        if (n && pos < re.size() && re[pos] == ')') {
            auto r = make_shared<_preRead_node>();
            r->left = n;
            r->flag = re[pos + 2] == '!';
            return r;
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

shared_ptr<_astNode> _ast::charSet_term(const wstring& re) {
    if (pos + 4 < re.size() && re[pos] == '(' && re[pos] + 1 == '?' && re[pos + 2] == ':') {
        //case 1 : (?: ) 
        pos += 3; // match '(?:'
        auto r = re_term(re);  
        if (r && pos < re.size() && re[pos] == ')') {
            ++pos;      //match ')'
            return r;
        } else {
            return nullptr;
        }
    } else if (pos < re.size() && re[pos] == '(') {
        //case 2 : ()
        ++pos;    // match'('
        auto r = make_shared<_catch_node>();
        auto n = re_term(re);
        if (n && pos < re.size() && re[pos] == ')') {
            ++pos;  //match')'
            r->catchIndex = catchArray.size();
            catchArray.push_back(vector<wstring>());
            r->left = n;
            return r;
        } else {
            err(pos, re);
            return nullptr;
        }
    } else if(pos + 2 < re.size() && re[pos] == '[') {
        return make_shared<_charSet_node>();        
    } else if (pos + 7 < re.size() && re[pos] == '(' && re[pos + 1] == '?' && re[pos + 2] == 'P' && re[pos + 3] == '<') {
        pos += 3;  //match '(?P<'
        auto r = make_shared<_cat_node>(); 
        wstring catchName;
        while (pos < re.size() && re[pos] != '>') {
            if (re[pos] == '\\') {
                ++pos;
            } 
            catchName.push_back(re[pos]);
            ++pos;
        }    // match'name>'
        if (pos < re.size()) {
            ++pos;
            auto n = re_term(re);
            r->left = n;
            if (pos < re.size() && re[pos] == ')') {
                ++pos;    //match ')'
            } else {
                err(pos, re);
                return nullptr;
            }
        } else {
            err(pos, re);
            return nullptr;
        }
        catchArray.push_back(vector<wstring>());
        nameMap[catchName] = catchArray.size() - 1;
        return r;
    } else {
        return nullptr;
    }
}

wchar_t _ast::_cat_start_marsk[] = {'{','}', ')', ']', '?', '*', '+', '|'};


