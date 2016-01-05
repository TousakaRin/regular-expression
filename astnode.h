#ifndef _AST_NODE_H_
#define _AST_NODE_H_

#include <string>
#include <memory>
#include <vector>

namespace rgx {

class _astNode {
public:
    std::shared_ptr<_astNode> pre_read, post_read;
    std::shared_ptr<_astNode> left, right;
    virtual ~_astNode();  
};

class _or_node : public _astNode {

};

class _charSet_node: public _astNode {
public:    
    _charSet_node(wchar_t);
    _charSet_node();
};

class _cat_node : public _astNode {

};


class _preRead_node : public _astNode {
public:
    bool flag;
};

class _catch_node : public _astNode {
public:    
    unsigned int catchIndex;
    std::shared_ptr<std::wstring> name;
};

}
#endif
