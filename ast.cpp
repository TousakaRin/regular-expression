#include "ast.h"
#include "stringTools.h"
#include <locale>
#include <cassert>

using namespace std;
using namespace rgx;

void _ast::err() {
    cout << "----------------------------------------" << endl;
    cout << "语法错误哟~" << endl;
    cout << "position :  " << pos << endl;
    if (pos < re.size() - 1) {
        cout << wstring_to_utf8(re.substr(0, pos)) << RED << wstring_to_utf8(re.substr(pos, 1)) << RESET << wstring_to_utf8(re.substr(pos + 1)) << endl;
    } else if (pos < re.size()){
        cout << wstring_to_utf8(re.substr(0, pos)) << RED << wstring_to_utf8(re.substr(pos, 1));
    } else {
        cout << wstring_to_utf8(re.substr(0, pos));
    }
    cout << "----------------------------------------" << endl;
}

_ast::_ast(const wstring &regular_expression) : re(regular_expression), pos(0) {
    root = re_term();
    if (pos != re.size()) {
        // 虽然re不用到达正则末尾，但是这里必须到达末尾，否则正则就是非法的
//        cout <<  "_ast throws" << endl;
//        err();
        root = nullptr;
    }
}

shared_ptr<_astNode> _ast::re_term() {
    if (pos >= re.size()) {
//        cout << "re_term throws 1" << endl;
        err();
        return nullptr;
    } 
    auto r = or_term();         //r 始终为子树的根节点
    while (pos < re.size() && re[pos] == '|') {
        ++pos;           // match '|'
        if (pos >= re.size()) {
            cout << "re_term throws 2" << endl;
            err();
            return nullptr;
        }
        auto n = or_term();
        if (n) {
            auto newRoot = make_shared<_or_node>();
            newRoot->left = r;
            newRoot->right = n;
            r = newRoot;
        } else {
            //err
            return nullptr;
        }
    }
    /*
     即使是re_term也可以不到达字符串末尾啊！！！
     因为在处理小括号时直接调用re_term处理了- -
     比如abc(ef|cd)ghi
    */
    return r;
}

shared_ptr<_astNode> _ast::or_term() {
    if (pos >= re.size()) {
//        cout << "or_term throws 1" << endl;
        err();
        return nullptr;
    }
    auto r = cat_term(); 
    while (pos < re.size()) {
        for (auto c : _cat_start_mask) {
            if (re[pos] == c) {
                //此处不应有err()
                return r;
            }
        }
        auto n = cat_term();
        if (n) {
            auto newRoot = make_shared<_cat_node>();
            newRoot->left = r;
            newRoot->right = n;
            r = newRoot;
        } else {
//            cout << "or_term throws 2" << endl;
//            err();
            return nullptr;
        }
    } 
    return r;
}

shared_ptr<_astNode> _ast::cat_term() {
    // cat_term 可以为null
    if (pos >= re.size()) {
        return nullptr;
    }
    auto pre = pre_read_term();
    auto r = charSet_term();
    if (!r) {
        return nullptr;
    }
    auto numt = num_term();
    auto post = post_read_term();
    if (numt) {
        r->numCount = numt;
        r->pre_read = pre; 
        r->post_read = post;
        return r;
    } else {
        return nullptr;
    }
}

shared_ptr<_astNode> _ast::pre_read_term() {
    //pre_read_term 可以为nullptr
    if (pos + 5 >= re.size()) {  //至少6个字符，如: (?<!x)
        return nullptr;
    } else if (re[pos] == '(' && re[pos + 1] == '?' && re[pos + 2] == '<' && (re[pos + 3] == '!' || re[pos + 3] == '=')) {
        pos += 3; //match '(?<'
        auto r = make_shared<_preRead_node>(re[pos] == '=');
        ++pos;     // match '=','!'
        /********************************/
        auto n = re_term(); //这里应该使用DFA处理，用来预读的子正则表达式讲道理的话支持纯正则特性就可以了
        /*******************************/
        if (n && pos < re.size() && re[pos] == ')') {
            ++pos;  //match ')'
            r->dfaTree = n;
            return r;
        } else if (pos >= re.size() || re[pos] != ')') {
            err();
            return nullptr;
        } else if (!n) {
            return nullptr;
        } else {
            // 此处应该永远不可达，因为 n, pos < re.size(), re[pos] 都已经全部测试过了
            // 加上这个是为了消除警告'控制流达到非void函数的末尾'
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

shared_ptr<_astNode> _ast::post_read_term() {
    //post_read_term 可以为nullptr
    if (pos + 4 >= re.size()) {  //至少6个字符，如: (?<!x)
        return nullptr;
    } else if (re[pos] == '(' && re[pos + 1] == '?'  && (re[pos + 2] == '!' || re[pos + 2] == '=')) {
        pos += 2; //match '(?'
        auto r = make_shared<_preRead_node>(re[pos] == '=');
        ++pos;     // match '=','!'
        /********************************/
        auto n = re_term(); //这里应该使用DFA处理，用来预读的子正则表达式讲道理的话支持纯正则特性就可以了
        /*******************************/
        if (n && pos < re.size() && re[pos] == ')') {
            ++pos;  //match ')'
            r->dfaTree = n;
            return r;
        } else if (pos >= re.size() || re[pos] != ')') {
            err();
            return nullptr;
        } else if (!n) {
            return nullptr;
        } else {
            // 此处应该永远不可达，因为 n, pos < re.size(), re[pos] 都已经全部测试过了
            // 加上这个是为了消除警告'控制流达到非void函数的末尾'
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

shared_ptr<_astNode> _ast::charSet_term() {
    if (pos + 4 < re.size() && re[pos] == '(' && re[pos + 1] == '?' && re[pos + 2] == ':') {
        //case 1 : (?: ) 
        return normalBracket();
    } else if (pos + 7 < re.size() && re[pos] == '(' && re[pos + 1] == '?' && re[pos + 2] == 'P' && re[pos + 3] == '<') {
        //case 2 : (?P<name> )
        return namedCatch();
    } else if (pos < re.size() && re[pos] == '(') {
        //case 3 : ()
        return unnamedCatch();
    } else if(pos + 2 < re.size() && re[pos] == '[') {
        //case 4 : [^-a-bf-i]
        return charClass();
    } else if (pos < re.size() && pos == '\\') {
        //case 5 : \xxx
        return normalTrans();
    } else {
        // single char
        auto r = make_shared<_charSet_node>(re[pos]);
        ++pos;
        return r;
    }
}

shared_ptr<_numCount_node>  _ast::num_term() {
    if (pos >= re.size()) {
        return make_shared<_numCount_node>();
    }
    if (re[pos] == '{') {
        ++pos;
        int lower = getNum();
        if (lower< 0) {
            err();
            return nullptr;
        }
        if (re[pos] == '}') {
            // {56}  match
            return make_shared<_numCount_node>(lower, lower);
        }
        if (re[pos] == '-') {
            ++pos;
            int upper = getNum();
            if (upper < 0) {
                err();
                return nullptr;
            }
            if (pos < re.size() && re[pos] == '}') {
                ++pos;
                if (lower > upper) {
                    err();
                    return nullptr;
                }
                if (pos < re.size() && re[pos] == '?') {
                    ++pos; //match {45-83}?
                    return make_shared<_numCount_node>(lower, upper, false);
                } else {
                    return make_shared<_numCount_node>(lower, upper);
                }
            }
        } else {
            err();
            return nullptr;
        }
    } else if (re[pos] == '?') {
        ++pos;
        if (pos < re.size() && re[pos] == '?') {
            ++pos;
            return make_shared<_numCount_node>(0, 1, false);
        } else {
            return make_shared<_numCount_node>(0, 1);
        }
    } else if (re[pos] == '*') {
       ++pos;  //match '*'
       if (pos < re.size() && re[pos] == '?') {
           ++pos;  //match '*?'
           return make_shared<_numCount_node>(0, -1, false);
       } else {
           return make_shared<_numCount_node>(0, -1);
       }
    } else if (re[pos] == '+') {
        ++pos;  //match '+'
        if (pos < re.size() && re[pos] == '?') {
            ++pos;
            return make_shared<_numCount_node>(1, -1, false);
        } else {
            return make_shared<_numCount_node>(1, -1);
        }
    }
    // numCount 可以为空！！即只出现一次
    return make_shared<_numCount_node>();
}


_ast::~_ast() {
    //do nothing ~智能指针就是牛
}


bool _ast::charSetTrans(shared_ptr<_charSet_node> r) {
    /* 处理字符类中的转义
    *  单个的'/',以及无效的转义都会被当成语法错误
    *  在字符类中除'/', 在开头的'^', 不在开头的'-'之外的关键字全部失去意义，仅仅表示该字符
    *  即所有的'/'，所有在开头的'^', 不在开头的'-'都需要转义
    *  转义字符'/'用于使用预定义字符类'/s' ,'/S'...以及表示'//', '/^', '/-'
    *  在字符类中无法使用具名/匿名引用，因为关键字已经失去意义                              
    */
    ++pos;
    if (pos >= re.size()) {
        err();
        return false;
    }
    if (re[pos] == 'w') {
        r->addWordRange();
    } else if (re[pos] == 'W') {
        r->deleteWordRange();
    } else if (re[pos] == 'd') {
        r->addDigitRange();
    } else if (re[pos] == 'D') {
        r->delteDigitRange();
    } else if (re[pos] == 's') {
        r->addSpaceRang();
    } else if (re[pos] == 'S') {
        r->deleteSpaceRange();
    } else if (re[pos] == '\\') {
        r->addCharRange(pair<wchar_t, wchar_t>('\\', '\\' + 1));
    }
    return true;
}

shared_ptr<_astNode> _ast::normalBracket() {
    // 在函数调用之前检查前缀是否为(?:,  函数内部不再检查前缀
    pos += 3; // match '(?:'
    auto r = re_term();  
    if (r && pos < re.size() && re[pos] == ')') {
        ++pos;      //match ')'
        return r;
    } else if (pos == re.size() || re[pos] != ')'){
        err();
        return nullptr;
    } else {
        return nullptr;   //只在发现错误字符时报错，否则交给下级函数报错
                         // 即r == nullptr时，由re_term()函数报错
    }
}


shared_ptr<_astNode> _ast::namedCatch() {
   // 在函数调用之前检查前缀是否为 (?P< ,  函数内部不再检查前缀
    pos += 3;  //match '(?P<'
    auto r = make_shared<_catch_node>(); 
    wstring catchName;
    while (pos < re.size() && re[pos] != '>') {
        if (re[pos] == '\\') {
            ++pos;
        } 
        catchName.push_back(re[pos]);
        ++pos;
    }    // match'name'
    if (pos < re.size()) {
        ++pos;  //match '>'
        auto n = re_term();
        if (n && pos < re.size() && re[pos] == ')') {
            ++pos;    //match ')'
            r->left = n;
            r->catchIndex = catchArray.size();
            nameMap[catchName] = catchArray.size();
            catchArray.push_back(vector<wstring>());
            return r;
        } else if (pos >= re.size() || re[pos] != ')'){
            err();
            return nullptr;
        } else {
            return nullptr;
        }
    } else {
        err();
        return nullptr;
    }
}

shared_ptr<_astNode> _ast::unnamedCatch() {
    //进入函数之前判断前缀是(,并且不是(?
    ++pos;    // match'('
    auto r = make_shared<_catch_node>();
    auto n = re_term();
    if (n && pos < re.size() && re[pos] == ')') {
        ++pos;  //match')'
        r->catchIndex = catchArray.size();
        catchArray.push_back(vector<wstring>());
        r->left = n;
        return r;
    } else if (pos >= re.size() || re[pos] != ')') {
        err();
        return nullptr;
    } else {
        return nullptr;
    }
}

shared_ptr<_astNode> _ast::charClass() {
    //进入函数之前判断前缀
    auto r = make_shared<_charSet_node>();
    ++pos; //match '['

    //匹配在[]内作为第一个字符的'^'或'-'
    //之后的'^'都表示单个字符，'-'都表示范围，不表示范围的'-'需要转义
    if (pos < re.size() && re[pos] == '^') {
        r->setInversison();
        ++pos;  //match '^' : '[^'
    } 
    while(pos < re.size() && re[pos] != ']') {
        auto pre = re[pos];
        ++pos;  
        if (pos < re.size() && re[pos] == '\\') {
           //处理转义字符，字符类中的转义字符和字符类之外的转义处理不同 
            if (!charSetTrans(r)) {
                return nullptr;
            }
        }
        if (pos < re.size() && re[pos] == '-') {
            ++pos;
            r->addCharRange(pair<wchar_t, wchar_t>(pre, re[pos]));
            ++pos;
            return r;
        } else if (pos >= re.size()) {
            err();
            return nullptr;
        } else {
            r->addCharRange(pair<wchar_t, wchar_t>(pre, pre));
        }
    }
    if (pos < re.size() && re[pos] == ']') {
        ++pos;
        return r;
    } else {
        err();
        return nullptr;
    }
}

shared_ptr<_astNode> _ast::normalTrans() {
    // 进入函数之前判断前缀
    ++pos;           //match '\'
    auto r = make_shared<_charSet_node>();
    if (pos >= re.size()) {
        err();
        return nullptr;
    } else {
        // 检查转义是否有意义
        bool check = false;
        for (auto w : _normalTrans_set) {
            check = (w == re[pos]);
        }
        if (!check) {
            err();
            return nullptr;
        }
    }
    if (re[pos] == 'w') {
        r->addWordRange(); 
    } else if (re[pos] == 'W') {
        r->deleteWordRange();
    } else if (re[pos] == 'd') {
        r->addDigitRange();
    } else if (re[pos] == 'D') {
        r->delteDigitRange();
    } else if (re[pos] == 's') {
        r->addSpaceRang();
    } else if (re[pos] == 'S') {
        r->deleteSpaceRange();
    } else {
        r->addCharRange(pair<wchar_t, wchar_t>(re[pos], re[pos]));
    }
    ++pos;  // match tranChar
    return r;
}

int _ast::getNum() {
    if (pos >= re.size()) {
        return -1;
    }
    if (re[pos] < '0' || re[pos] > '9') {
        return -1;
    }
    int sum = 0;
    while (pos < re.size() && re[pos] >= '0' && re[pos] <= '9') {
        sum += re[pos] - '0';
        sum *= 10;
        ++pos;
    }
    return sum;
}

wchar_t _ast::_cat_start_mask[] = {'{','}', ')', ']', '?', '*', '+', '|'};

wchar_t _ast::_normalTrans_set[] = {'{', '}', '(', ')', '[', ']', '?', '*', '+', '|', '\\', 'w', 'W', 's', 'S', 'd', 'D'};


