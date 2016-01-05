#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <memory>
#include "matchObj.h"
#include "ast.h"
#include <string>

namespace rgx{

class pattern {
public:
    virtual std::shared_ptr<matchObj> match(const std::wstring&) = 0;
    virtual std::shared_ptr<matchObj> search(const std::wstring&) = 0;
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::wstring&) = 0;
};

class dfa_pattern : public pattern {
public:
    dfa_pattern (const _ast&);
};

class nfa_pattern : public pattern {
public:
   nfa_pattern (const _ast&); 
};

}

#endif
