#include "ast.h"
#include "stringTools.h"

using namespace std;
using namespace rgx;

void _ast::err() {
    cout << "------------------------------------------------------" << endl;
    cout << "语法错误哟~" << endl;
    cout << "position :  " << pos << endl;
    if (pos < re.size() - 1) {
        cout << wstring_to_utf8(re.substr(0, pos)) << RED << wstring_to_utf8(re.substr(pos, 1)) << RESET << wstring_to_utf8(re.substr(pos + 1)) << endl;
    } else if (pos < re.size()) {
        cout << wstring_to_utf8(re.substr(0, pos)) << RED << wstring_to_utf8(re.substr(pos, 1)) << RESET << endl;
    } else {
        cout << wstring_to_utf8(re.substr(0, pos)) << endl;
    }
    cout << "------------------------------------------------------" << endl << endl;
    exit(0);
}


_ast::_ast(const wstring &regular_expression) : re(regular_expression), pos(0) , catchNum(0) {
    root = re_term();
    if (root != nullptr && pos < re.size()) {
        err();
    }
}

shared_ptr<_astNode> _ast::re_term() {
    //re_term 可以为空
    auto r = or_term();         //r 始终为子树的根节点
    while (pos < re.size() && re[pos] == '|') {
        ++pos;           // match '|'
           if (pos >= re.size()) {
            err();
            return nullptr;
        }
        auto n = or_term();
        // 出现'|' 之后，下一个必须是非空的or_term
        if (n) {
            auto newRoot = make_shared<_or_node>();
            newRoot->left = r;
            newRoot->right = n;
            r = newRoot;
        } else {
            // or_term 是可以正常为空的，所以需要在这里报错
            err();
            return nullptr;
        }
    }
    /*
     * 若re_term()没有到达字符串末尾，只有一种情况
     * re被用来处理小括号了，如(?P<name> ) / ()
     * 否则就是出错了！！！
    */
    if (pos < re.size() && re[pos] != ')') {
        cout << "second" << endl;
        err();
        return nullptr;
    }
    return r;
}

shared_ptr<_astNode> _ast::or_term() {
    // or_term 可以为空 or_term -> cat_term -> null
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
            //控制流到达这里说明出现了cat_term的前缀，但是却解析出错了
            err();
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
    //判断是否属于位置节点^, $, \A...位置节点其实几乎可以
    //和普通节点一同处理，在这里与普通的字符节点分开处理,感觉
    //这样逻辑更加清晰一些
    shared_ptr<_astNode> r = position_term();
    if (!r) {
        //在cat_term中r若不是position_term，则r是charSet_term
        r = charSet_term();
    }
    auto numt = num_term();
    auto post = post_read_term();
    if (!numt) {
        // numt 必不为空，即使表达式中没有num_term, num_term()方法也回返回一个_numCount_node
        // 否则无法在charSet_term方法中正确识别 numCount 返回的错误---numCount本来就是可以为空的
        // 所以这里在numCount为空时返回默认值1
        err();
        return nullptr;
    } else if (!r && (post || pre)) {
        //如果charSet_term 为空但是预读不为空，则说明出现了裸露的预读字段，出错
        err();
        return nullptr;
    }
    if (numt->lower == 1 && numt->upper == 1 && !pre && ! post) {
        //若既没有正向预读，又没有反向预读，重复次数还为1，则没有生成numCount 节点的必要，直接返回r
        return r;
    } else {
        numt->left = r;
        numt->pre_read = pre;
        numt->pre_read = pre; 
        return numt;
    }
}

shared_ptr<_preRead_node> _ast::pre_read_term() {
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
        } else {
            err();
            return nullptr;
        }
    } else {
        //由于pre_read可以为空，所以前缀不满足或者长度不够时，直接return nullptr，不报错
        return nullptr;
    }
}

shared_ptr<_preRead_node> _ast::post_read_term() {
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
        } else {
            err();
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

shared_ptr<_astNode> _ast::charSet_term() {
    if (pos >= re.size()) {
        // charSet_term 可以为空
        return nullptr;
    }
    bool check = true;
    for (auto w : _charSet_mask) {
        if (re[pos] == w) {
            check = false;
        }
    }
    if (!check) {
        err();
        return nullptr;
    }

    if (pos + 4 < re.size() && re[pos] == '(' && re[pos + 1] == '?' && re[pos + 2] == ':') {
        //case 1 : (?: ) 
        return normalBracket();
    } else if (pos + 7 < re.size() && re[pos] == '(' && re[pos + 1] == '?' && re[pos + 2] == 'P' && re[pos + 3] == '<') {
        //case 2 : (?P<name> )
        return namedCatch();
    } else if (pos + 5 < re.size() && re[pos] == '(' && re[pos + 1] == '?' && re[pos + 2] == 'P' && re[pos + 3] == '=') {
        //case 3 : (?P=name)
        pos += 4;   //match '(?P='
        wstring name;
        while (pos < re.size() && re[pos] != ')') {
            if (re[pos] == '\\') {
                ++pos;
            }
            name.push_back(re[pos]);
            ++pos;
        }
        if (pos == re.size()) {
            err();
            return nullptr;
        }
        ++pos;  //match ')'
        return namedReference(name);

    } else if (pos < re.size() && re[pos] == '(') {
        //case 4 : ()
        return unnamedCatch();
    } else if(pos + 2 < re.size() && re[pos] == '[') {
        //case 5 : [^-a-bf-i]
        return charClass();
    } else if (pos < re.size() && re[pos] == '\\') {
        //case 6 : \xxx
        return normalTrans();
    } else {
        // single char
        // 处理'.', '.'出现在字符类中时，不具备特殊意义，所以只要在这里处理'.'
        if (re[pos] == '.') {
            auto  r = make_shared<_charSet_node>();
            r->setInversison();
            r->addCharRange(pair<wchar_t, wchar_t>(re[pos], re[pos]));
            ++pos;
            return r;
        }
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
            auto errPos = pos;
            ++pos;
            int upper = getNum();
            if (upper < 0) {
                err();
                return nullptr;
            }
            if (pos < re.size() && re[pos] == '}') {
                ++pos;
                if (lower > upper) {
                    pos = errPos;
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
    } else {
        //无意义的转义，语法错误
        err();
        return false;
    }
    ++pos;
    return true;
}

shared_ptr<_astNode> _ast::normalBracket() {
    // 在函数调用之前检查前缀是否为(?:,  函数内部不再检查前缀
    pos += 3; // match '(?:'
    auto r = re_term();  
    if (r && pos < re.size() && re[pos] == ')') {
        ++pos;      //match ')'
        return r;
    } else {
        err();
        return nullptr;
    } 
}


shared_ptr<_catch_node> _ast::namedCatch() {
   // 在函数调用之前检查前缀是否为 (?P< ,  函数内部不再检查前缀
    pos += 4;  //match '(?P<'
    auto r = make_shared<_catch_node>(); 
    while (pos < re.size() && re[pos] != '>') {
        if (re[pos] == '\\') {
            ++pos;
        } 
        r->name.push_back(re[pos]);
        ++pos;
    }    // match'name'
    if (pos < re.size()) {
        ++pos;  //match '>'
        auto n = re_term();
        if (n && pos < re.size() && re[pos] == ')') {
            ++pos;    //match ')'
            r->left = n;
            r->catchIndex = ++catchNum;
            nameMap[r->name] = catchNum;
            return r;
        } else {
            err();
            return nullptr;
        }
    } else {
        err();
        return nullptr;
    }
}

shared_ptr<_catch_node> _ast::unnamedCatch() {
    //进入函数之前判断前缀是(,并且不是(?
    ++pos;    // match'('
    auto r = make_shared<_catch_node>();
    auto n = re_term();
    if (n && pos < re.size() && re[pos] == ')') {
        ++pos;  //match')'
        r->catchIndex = ++catchNum;
        r->left = n;
        return r;
    } else {
        err();
        return nullptr;
    }
}

shared_ptr<_charSet_node> _ast::charClass() {
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
        if (pos < re.size() && re[pos] == '\\') {
           //处理转义字符，字符类中的转义字符和字符类之外的转义处理不同 
            if (!charSetTrans(r)) {
                err();
                return nullptr;
            }
        }
        auto pre = re[pos];
        ++pos;  
        if (pos < re.size() && re[pos] == '-') {
            ++pos;
            if (pre > re[pos]) {
                err();
                return nullptr;
            }
            r->addCharRange(pair<wchar_t, wchar_t>(pre, re[pos]));
            ++pos;
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
    } 

    // 检查转义是否有意义
    bool check = false;
    for (auto w : _normalTrans_set) {
        check = check || (w == re[pos]);
    }
    if (!check) {
        err();
        return nullptr;
    }

    if (re[pos] == '<') {
        return unnamedReference();
    } else if (re[pos] == 'w') {
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
        //转义关键字
        r->addCharRange(pair<wchar_t, wchar_t>(re[pos], re[pos]));
    }
    ++pos;  // match tranChar
    return r;
}

int _ast::getNum() {
    //正则表达式中不会出现需要提取的、负的整数
    if (pos >= re.size()) {
        return -1;
    }
    if (re[pos] < '0' || re[pos] > '9') {
        return -1;
    }
    int sum = 0;
    while (pos < re.size() && re[pos] >= '0' && re[pos] <= '9') {
        sum *= 10;
        sum += re[pos] - '0';
        ++pos;
    }
    return sum;
}

shared_ptr<_reference_node> _ast::unnamedReference() {
    //  '\<number>'
    ++pos;  //match '<'
    int index  = getNum(); //getNum
    if (index < 0 || static_cast<unsigned int>(index) > catchNum) {
        err();
        return nullptr;
    }
    if (pos >= re.size() || re[pos] != '>') {
        err();
        return nullptr;
    }
    ++pos;  //match '>'
    return make_shared<_reference_node>(index);
}


shared_ptr<_reference_node> _ast::namedReference(const wstring &name) {
    if (nameMap.find(name) == nameMap.end()) {
        err();
        return nullptr;
    }
    return make_shared<_reference_node>(nameMap[name], name);
}

shared_ptr<_position_node> _ast::position_term() {
    //本来生成位置节点时应该按照节点的类型对位置和数量信息进行精简
    //这部分工作交给生成自动机时在执行，为什么。。。因为生成ast的时候
    //所有的节点都用的_astNode类型啊，哭
    if (pos >= re.size()) {
        return nullptr;
    }
    if (re[pos] == '^') {
        ++pos;
        return make_shared<_position_node>(_position_node::LINE_BEGIN);
    } else if (re[pos] == '$') {
        ++pos;
        return make_shared<_position_node>(_position_node::LINE_END);
    } else if (pos + 1 < re.size() && re[pos] == '\\' && re[pos + 1] == 'A') {
        pos += 2;
        return make_shared<_position_node>(_position_node::STRING_BEGIN);
    } else if (pos + 1 < re.size() && re[pos] == '\\' && re[pos + 1] == 'Z') {
        pos += 2;
        return make_shared<_position_node>(_position_node::STRING_END);
    } else if (pos + 1 < re.size() && re[pos] == '\\' && re[pos + 1] == 'b') {
        pos += 2;
        return make_shared<_position_node>(_position_node::BREAK_OFF);
    } else if (pos + 1 < re.size() && re[pos] == '\\' && re[pos + 1] == 'B') {
        pos += 2;
        return make_shared<_position_node>(_position_node::NO_BREAK_OFF);
    } else {
        return nullptr;
    }
}
wchar_t _ast::_cat_start_mask[] = {'{','}', ')', ']', '?', '*', '+', '|'};

wchar_t _ast::_normalTrans_set[] = {'{', '}', '(', ')', '[', ']', '?', '*', '+', '|', '\\', 'w', 'W', 's', 'S', 'd', 'D', '<', '^', '$'};

wchar_t _ast::_charSet_mask[] = {'{', '}', '?', '*', '+', '|'};
