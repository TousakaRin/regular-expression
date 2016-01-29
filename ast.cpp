#include "ast.h"
#include "stringTools.h"

using namespace std;
using namespace rgx;

void rgx::_ast::err() {
    cout << "------------------------------------------------------" << endl;
    cout << "语法错误哟~" << endl;
    cout << "_position :  " << _pos << endl;
    if (_pos < _re.size() - 1) {
        cout << ucs2_to_string(_re.substr(0, _pos)) << RED << ucs2_to_string(_re.substr(_pos, 1)) << RESET << ucs2_to_string(_re.substr(_pos + 1)) << endl;
    } else if (_pos < _re.size()) {
        cout << ucs2_to_string(_re.substr(0, _pos)) << RED << ucs2_to_string(_re.substr(_pos, 1)) << RESET << endl;
    } else {
        cout << ucs2_to_string(_re.substr(0, _pos)) << endl;
    }
    cout << "------------------------------------------------------" << endl << endl;
    exit(0);
}


rgx::_ast::_ast(const string &_regular_expression) : _build_type(build_to_dfa), _re(string_to_ucs2(_regular_expression)), _pos(0) , _captureIndex(0) , edgeMgr(make_shared<edgeManager>()) {
    _root = re_term();
    if (_root != nullptr && _pos < _re.size()) {
        err();
    }
}

shared_ptr<_astNode> rgx::_ast::re_term() {
    //_re_term 可以为空
    auto r = or_term();         //r 始终为子树的根节点
    while (_pos < _re.size() && _re[_pos] == '|') {
        ++_pos;           // match '|'
           if (_pos >= _re.size()) {
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
     * _re被用来处理小括号了，如(?P<name> ) / ()
     * 否则就是出错了！！！
    */
    if (_pos < _re.size() && _re[_pos] != ')') {
        cout << "second" << endl;
        err();
        return nullptr;
    }
    return r;
}

shared_ptr<_astNode> rgx::_ast::or_term() {
    // or_term 可以为空 or_term -> cat_term -> null
    auto r = cat_term(); 
    while (_pos < _re.size()) {
        for (auto c : _cat_start_mask) {
            if (_re[_pos] == c) {
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

shared_ptr<_astNode> rgx::_ast::cat_term() {
    // cat_term 可以为null
    if (_pos >= _re.size()) {
        return nullptr;
    }
    auto pre = pre_read_term();
    //判断是否属于位置节点^, $, \A...位置节点其实几乎可以
    //和普通节点一同处理，在这里与普通的字符节点分开处理,感觉
    //这样逻辑更加清晰一些
    shared_ptr<_astNode> r = position_term();
    if (!r) {
        //在cat_term中r若不是_position_term，则r是charSet_term
        r = charSet_term();
    }
    auto numt = num_term();
    auto _post = post_read_term();
    if (!numt) {
        // numt 必不为空，即使表达式中没有num_term, num_term()方法也回返回一个_numCount_node
        // 否则无法在charSet_term方法中正确识别 numCount 返回的错误---numCount本来就是可以为空的
        // 所以这里在numCount为空时返回默认值1
        err();
        return nullptr;
    } else if (!r && (_post || pre)) {
        //如果charSet_term 为空但是预读不为空，则说明出现了裸露的预读字段，出错
        err();
        return nullptr;
    }
    if (numt->lower == 1 && numt->upper == 1 && !pre && ! _post) {
        //若既没有正向预读，又没有反向预读，重复次数还为1，则没有生成numCount 节点的必要，直接返回r
        return r;
    } else {
        numt->left = r;
        numt->pre_read = pre;
        numt->post_read = _post; 
        return numt;
    }
}

shared_ptr<_preRead_node> rgx::_ast::pre_read_term() {
    //pre__read_term 可以为nullptr
    if (_pos + 5 >= _re.size()) {  //至少6个字符，如: (?<!x)
        return nullptr;
    } else if (_re[_pos] == '(' && _re[_pos + 1] == '?' && _re[_pos + 2] == '<' && (_re[_pos + 3] == '!' || _re[_pos + 3] == '=')) {
        _pos += 3; //match '(?<'
        auto r = make_shared<_preRead_node>(_re[_pos] == '=');
        ++_pos;     // match '=','!'
        /********************************/
        auto n = re_term(); //这里应该使用DFA处理，用来预读的子正则表达式讲道理的话支持纯正则特性就可以了
        /*******************************/
        if (n && _pos < _re.size() && _re[_pos] == ')') {
            ++_pos;  //match ')'
            r->dfaTree = n;
            return r;
        } else {
            err();
            return nullptr;
        }
    } else {
        //由于pre__read可以为空，所以前缀不满足或者长度不够时，直接return nullptr，不报错
        return nullptr;
    }
}

shared_ptr<_preRead_node> rgx::_ast::post_read_term() {
    //_post__read_term 可以为nullptr
    if (_pos + 4 >= _re.size()) {  //至少6个字符，如: (?<!x)
        return nullptr;
    } else if (_re[_pos] == '(' && _re[_pos + 1] == '?'  && (_re[_pos + 2] == '!' || _re[_pos + 2] == '=')) {
        _pos += 2; //match '(?'
        auto r = make_shared<_preRead_node>(_re[_pos] == '=');
        ++_pos;     // match '=','!'
        /********************************/
        auto n = re_term(); //这里应该使用DFA处理，用来预读的子正则表达式讲道理的话支持纯正则特性就可以了
        /*******************************/
        if (n && _pos < _re.size() && _re[_pos] == ')') {
            ++_pos;  //match ')'
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

shared_ptr<_astNode> rgx::_ast::charSet_term() {
    if (_pos >= _re.size()) {
        // charSet_term 可以为空
        return nullptr;
    }
    bool check = true;
    for (auto w : _charSet_mask) {
        if (_re[_pos] == w) {
            check = false;
        }
    }
    if (!check) {
        err();
        return nullptr;
    }

    if (_pos + 4 < _re.size() && _re[_pos] == '(' && _re[_pos + 1] == '?' && _re[_pos + 2] == ':') {
        //case 1 : (?: ) 
        return normalBracket();
    } else if (_pos + 7 < _re.size() && _re[_pos] == '(' && _re[_pos + 1] == '?' && _re[_pos + 2] == 'P' && _re[_pos + 3] == '<') {
        //case 2 : (?P<name> )
        return namedCapture();
    } else if (_pos + 5 < _re.size() && _re[_pos] == '(' && _re[_pos + 1] == '?' && _re[_pos + 2] == 'P' && _re[_pos + 3] == '=') {
        //case 3 : (?P=name)
        _pos += 4;   //match '(?P='
        u16string name;
        while (_pos < _re.size() && _re[_pos] != ')') {
            if (_re[_pos] == '\\') {
                ++_pos;
            }
            name.push_back(_re[_pos]);
            ++_pos;
        }
        if (_pos == _re.size()) {
            err();
            return nullptr;
        }
        ++_pos;  //match ')'
        return namedReference(name);

    } else if (_pos < _re.size() && _re[_pos] == '(') {
        //case 4 : ()
        return unnamedCapture();
    } else if(_pos + 2 < _re.size() && _re[_pos] == '[') {
        //case 5 : [^-a-bf-i]
        return charClass();
    } else if (_pos < _re.size() && _re[_pos] == '\\') {
        //case 6 : \xxx
        return normalTrans();
    } else {
        // single char
        // 处理'.', '.'出现在字符类中时，不具备特殊意义，所以只要在这里处理'.'
        if (_re[_pos] == '.') {
            auto  r = make_shared<_charSet_node>();
            r->setInversison();
            r->addCharRange(pair<char16_t, char16_t>(_re[_pos], _re[_pos] + 1), edgeMgr);
            ++_pos;
            return r;
        }
        auto r = make_shared<_charSet_node>();
        r->addCharRange(pair<char16_t, char16_t>(_re[_pos], _re[_pos] + 1), edgeMgr);
        ++_pos;
        return r;
    }
}

shared_ptr<_numCount_node>  rgx::_ast::num_term() {
    if (_pos >= _re.size()) {
        return make_shared<_numCount_node>();
    }
    if (_re[_pos] == '{') {
        ++_pos;
        int lower = getNum();
        if (lower< 0) {
            err();
            return nullptr;
        }
        if (_re[_pos] == '}') {
            // {56}  match
            return make_shared<_numCount_node>(lower, lower);
        }
        if (_re[_pos] == '-') {
            auto errPos = _pos;
            ++_pos;
            int upper = getNum();
            if (upper < 0) {
                err();
                return nullptr;
            }
            if (_pos < _re.size() && _re[_pos] == '}') {
                ++_pos;
                if (lower > upper) {
                    _pos = errPos;
                    err();
                    return nullptr;
                }
                if (_pos < _re.size() && _re[_pos] == '?') {
                    ++_pos; //match {45-83}?
                    return make_shared<_numCount_node>(lower, upper, false);
                } else {
                    return make_shared<_numCount_node>(lower, upper);
                }
            }
        } else {
            err();
            return nullptr;
        }
    } else if (_re[_pos] == '?') {
        ++_pos;
        if (_pos < _re.size() && _re[_pos] == '?') {
            ++_pos;
            return make_shared<_numCount_node>(0, 1, false);
        } else {
            return make_shared<_numCount_node>(0, 1);
        }
    } else if (_re[_pos] == '*') {
       ++_pos;  //match '*'
       if (_pos < _re.size() && _re[_pos] == '?') {
           ++_pos;  //match '*?'
           return make_shared<_numCount_node>(0, -1, false);
       } else {
           return make_shared<_numCount_node>(0, -1);
       }
    } else if (_re[_pos] == '+') {
        ++_pos;  //match '+'
        if (_pos < _re.size() && _re[_pos] == '?') {
            ++_pos;
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


bool rgx::_ast::charSetTrans(shared_ptr<_charSet_node> r) {
    /* 处理字符类中的转义
    *  单个的'/',以及无效的转义都会被当成语法错误
    *  在字符类中除'/', 在开头的'^', 不在开头的'-'之外的关键字全部失去意义，仅仅表示该字符
    *  即所有的'/'，所有在开头的'^', 不在开头的'-'都需要转义
    *  转义字符'/'用于使用预定义字符类'/s' ,'/S'...以及表示'//', '/^', '/-'
    *  在字符类中无法使用具名/匿名引用，因为关键字已经失去意义                              
    */
    ++_pos;
    if (_pos >= _re.size()) {
        err();
        return false;
    }
    if (_re[_pos] == 'w') {
        r->addWordRange(edgeMgr);
    } else if (_re[_pos] == 'W') {
        r->addUWordRange();
    } else if (_re[_pos] == 'd') {
        r->addDigitRange(edgeMgr);
    } else if (_re[_pos] == 'D') {
        r->addUDigitRange();
    } else if (_re[_pos] == 's') {
        r->addSpaceRang(edgeMgr);
    } else if (_re[_pos] == 'S') {
        r->addUSpaceRange();
    } else if (_re[_pos] == '\\') {
        r->addCharRange(pair<char16_t, char16_t>('\\', '\\' + 1), edgeMgr);
    } else {
        //无意义的转义，语法错误
        err();
        return false;
    }
    ++_pos;
    return true;
}

shared_ptr<_astNode> rgx::_ast::normalBracket() {
    // 在函数调用之前检查前缀是否为(?:,  函数内部不再检查前缀
    _pos += 3; // match '(?:'
    auto r = re_term();  
    if (r && _pos < _re.size() && _re[_pos] == ')') {
        ++_pos;      //match ')'
        return r;
    } else {
        err();
        return nullptr;
    } 
}


shared_ptr<_capture_node> rgx::_ast::namedCapture() {
   // 在函数调用之前检查前缀是否为 (?P< ,  函数内部不再检查前缀
    _pos += 4;  //match '(?P<'
    auto r = make_shared<_capture_node>(); 
    while (_pos < _re.size() && _re[_pos] != '>') {
        if (_re[_pos] == '\\') {
            ++_pos;
        } 
        r->name.push_back(_re[_pos]);
        ++_pos;
    }    // match'name'
    if (_pos < _re.size()) {
        ++_pos;  //match '>'
        auto n = re_term();
        if (n && _pos < _re.size() && _re[_pos] == ')') {
            ++_pos;    //match ')'
            r->left = n;
            r->captureIndex = ++_captureIndex;
            _nameMap[r->name] = _captureIndex;
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

shared_ptr<_capture_node> rgx::_ast::unnamedCapture() {
    //进入函数之前判断前缀是(,并且不是(?
    ++_pos;    // match'('
    auto r = make_shared<_capture_node>();
    auto n = re_term();
    if (n && _pos < _re.size() && _re[_pos] == ')') {
        ++_pos;  //match')'
        r->captureIndex = ++_captureIndex;
        r->left = n;
        return r;
    } else {
        err();
        return nullptr;
    }
}

shared_ptr<_charSet_node> rgx::_ast::charClass() {
    //进入函数之前判断前缀
    auto r = make_shared<_charSet_node>();
    ++_pos; //match '['

    //匹配在[]内作为第一个字符的'^'或'-'
    //之后的'^'都表示单个字符，'-'都表示范围，不表示范围的'-'需要转义
    if (_pos < _re.size() && _re[_pos] == '^') {
        r->setInversison();
        ++_pos;  //match '^' : '[^'
    } 
    while(_pos < _re.size() && _re[_pos] != ']') {
        if (_pos < _re.size() && _re[_pos] == '\\') {
           //处理转义字符，字符类中的转义字符和字符类之外的转义处理不同 
            if (!charSetTrans(r)) {
                err();
                return nullptr;
            } else {
                continue;
            }
        }
        auto pre = _re[_pos];
        ++_pos;  
        if (_pos < _re.size() && _re[_pos] == '-') {
            ++_pos;
            if (pre > _re[_pos]) {
                err();
                return nullptr;
            }
            r->addCharRange(pair<char16_t, char16_t>(pre, _re[_pos] + 1), edgeMgr);
            ++_pos;
        } else if (_pos >= _re.size()) {
            err();
            return nullptr;
        } else {
            r->addCharRange(pair<char16_t, char16_t>(pre, pre + 1), edgeMgr);
        }
    }
    if (_pos < _re.size() && _re[_pos] == ']') {
        ++_pos;
        return r;
    } else {
        err();
        return nullptr;
    }
}

shared_ptr<_astNode> rgx::_ast::normalTrans() {
    // 入函数之前判断前缀
    ++_pos;           //match '\'
    auto r = make_shared<_charSet_node>();
    if (_pos >= _re.size()) {
        err();
        return nullptr;
    } 

    // 检查转义是否有意义
    bool check = false;
    for (auto w : _normalTrans_set) {
        check = check || (w == _re[_pos]);
    }
    if (!check) {
        err();
        return nullptr;
    }

    if (_re[_pos] == '<') {
        return unnamedReference();
    } else if (_re[_pos] == 'w') {
        r->addWordRange(edgeMgr); 
    } else if (_re[_pos] == 'W') {
        r->addUWordRange();
    } else if (_re[_pos] == 'd') {
        r->addDigitRange(edgeMgr);
    } else if (_re[_pos] == 'D') {
        r->addUDigitRange();
    } else if (_re[_pos] == 's') {
        r->addSpaceRang(edgeMgr);
    } else if (_re[_pos] == 'S') {
        r->addUSpaceRange();
    } else {
        //转义关键字
        r->addCharRange(pair<char16_t, char16_t>(_re[_pos], _re[_pos] + 1), edgeMgr);
    }
    ++_pos;  // match tranChar
    return r;
}

int rgx::_ast::getNum() {
    //正则表达式中不会出现需要提取的、负的整数
    if (_pos >= _re.size()) {
        return -1;
    }
    if (_re[_pos] < '0' || _re[_pos] > '9') {
        return -1;
    }
    int sum = 0;
    while (_pos < _re.size() && _re[_pos] >= '0' && _re[_pos] <= '9') {
        sum *= 10;
        sum += _re[_pos] - '0';
        ++_pos;
    }
    return sum;
}

shared_ptr<_reference_node> rgx::_ast::unnamedReference() {
    //  '\<number>'
    ++_pos;  //match '<'
    int index  = getNum(); //getNum
    if (index < 0 || static_cast<unsigned int>(index) > _captureIndex) {
        err();
        return nullptr;
    }
    if (_pos >= _re.size() || _re[_pos] != '>') {
        err();
        return nullptr;
    }
    ++_pos;  //match '>'
    return make_shared<_reference_node>(index);
}


shared_ptr<_reference_node> rgx::_ast::namedReference(const u16string &name) {
    if (_nameMap.find(name) == _nameMap.end()) {
        err();
        return nullptr;
    }
    return make_shared<_reference_node>(_nameMap[name], name);
}

shared_ptr<_position_node> rgx::_ast::position_term() {
    //本来生成位置节点时应该按照节点的类型对位置和数量信息进行精简
    //这部分工作交给生成自动机时在执行，为什么。。。因为生成ast的时候
    //所有的节点都用的_astNode类型啊，哭
    if (_pos >= _re.size()) {
        return nullptr;
    }
    if (_re[_pos] == '^') {
        ++_pos;
        return make_shared<_position_node>(_position_node::LINE_BEGIN);
    } else if (_re[_pos] == '$') {
        ++_pos;
        return make_shared<_position_node>(_position_node::LINE_END);
    } else if (_pos + 1 < _re.size() && _re[_pos] == '\\' && _re[_pos + 1] == 'A') {
        _pos += 2;
        return make_shared<_position_node>(_position_node::STRING_BEGIN);
    } else if (_pos + 1 < _re.size() && _re[_pos] == '\\' && _re[_pos + 1] == 'Z') {
        _pos += 2;
        return make_shared<_position_node>(_position_node::STRING_END);
    } else if (_pos + 1 < _re.size() && _re[_pos] == '\\' && _re[_pos + 1] == 'b') {
        _pos += 2;
        return make_shared<_position_node>(_position_node::BREAK_OFF);
    } else if (_pos + 1 < _re.size() && _re[_pos] == '\\' && _re[_pos + 1] == 'B') {
        _pos += 2;
        return make_shared<_position_node>(_position_node::NO_BREAK_OFF);
    } else {
        return nullptr;
    }
}


char16_t rgx::_ast::_cat_start_mask[] = {'{','}', ')', ']', '?', '*', '+', '|'};

char16_t rgx::_ast::_normalTrans_set[] = {'{', '}', '(', ')', '[', ']', '?', '*', '+', '|', '\\', 'w', 'W', 's', 'S', 'd', 'D', '<', '^', '$'};

char16_t rgx::_ast::_charSet_mask[] = {'{', '}', '?', '*', '+', '|'};

