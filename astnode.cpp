#include "astnode.h"
#include "stringTools.h"
#include "nfaEdge.h"
#include "nfaNode.h"
#include <iostream>

using namespace std;
using namespace rgx;



/*-----------------------------------------------*/

rgx::_astNode::_astNode() : _nfa(nullptr) {

}

string rgx::_astNode::toString() {
    string info;
    info += " astnode \n\n";
    return info;
}

void rgx::_astNode::generateNFA() {
    cout << "\nan error occured !!!!\ninformation:" << endl;
    cout << toString();
}

void rgx::_astNode::err() {
    //出错时简单暴力的直接崩溃
    exit(-1);
}

void rgx::_astNode::err(const string& errMsg) {
    cout << "\n\n" << errMsg << endl;
    err();
}

rgx::_astNode::~_astNode() {
    //do nothing
}

/*-----------------------------------------------*/


string rgx::_or_node::toString() {
    string info;
    info += " or_node \n\n";
    return info;
}


void rgx::_or_node::generateNFA() {
    // 首先新建开始与完成节点
    cout << "or_node_generate_NFA\n" << endl;
    auto startNode = make_shared<_NFA_Node>(); 
    auto finishNode = make_shared<_NFA_Node>();

    if (!_left || !_left->_nfa) {
        cout << toString() << endl;
        err(" 生成nfa时出错 ");
    }
    if (!_right || !_right->_nfa) {
        cout << toString() << endl;
        err(" 生成nfa时出错 ");
    }

    //从startNode出发，新建两条epsilon边分别指向两个子节点NFA
    startNode->addEpsilonEdge(_left->_nfa->first);
    startNode->addEpsilonEdge(_right->_nfa->first);

    //将两个子节点的finishNode各新建一条epsilon边指向生成的finishNode
    _left->_nfa->second->addEpsilonEdge(finishNode);
    _right->_nfa->second->addEpsilonEdge(finishNode);

    _nfa = make_shared<_NFA>(startNode, finishNode); 
}

/*-----------------------------------------------*/

rgx::_charSet_node::_charSet_node(shared_ptr<_edgeManager> edgemgr) : _delOPT(0), _edgeMgr(edgemgr), inversion(false) {

}

rgx::_charSet_node::_charSet_node(char16_t c) : inversion(false) {
    cout << "-------------------" << endl;
    cout << " err construction " << c << endl;
    cout << "-------------------" << endl;
}


void rgx::_charSet_node::addCharRange(const pair<char16_t, char16_t>& cr) {
    if (auto p = _edgeMgr.lock()) {
        p->addRange(cr);
        _acceptSet.push_back(cr);
    } else {
        err(" 找不到对象: _edgeMgr");
    }
}


void rgx::_charSet_node::setInversison() {
    inversion = true;
}


void rgx::_charSet_node::addWordRange() {
    //左闭右开区间~~~~~
    if (auto p = _edgeMgr.lock()) {
        addCharRange(pair<char16_t, char16_t>('a', 'z' + 1));
        addCharRange(pair<char16_t, char16_t>('A', 'Z' + 1));
        addCharRange(pair<char16_t, char16_t>('0', '9' + 1));
    } else {
        err("in function rgx::_charSet_node::addWordRange()  \n\n 找不到对象: _edgeMgr \n\n");
    }
}

void rgx::_charSet_node::addUWordRange() {
    _delOPT |= NO_WORD;
}

void rgx::_charSet_node::addSpaceRang() {
    if (auto p = _edgeMgr.lock()) {
        addCharRange(pair<char16_t, char16_t>('\t', '\t' + 1));
        addCharRange(pair<char16_t, char16_t>('\r', '\r' + 1));
        addCharRange(pair<char16_t, char16_t>('\n', '\n' + 1));
        addCharRange(pair<char16_t, char16_t>('\b', '\b' + 1));
        addCharRange(pair<char16_t, char16_t>('\f', '\f' + 1));
        addCharRange(pair<char16_t, char16_t>('\v', '\v' + 1));
    } else {
        err("in function rgx::_charSet_node::addSpaceRange()  \n\n 找不到对象: _edgeMgr \n\n");
    }
}

void rgx::_charSet_node::addUSpaceRange() {
    _delOPT |= NO_SPACE;
}


void rgx::_charSet_node::addDigitRange() {
    addCharRange(pair<char16_t, char16_t>('0', '9' + 1));
}

void rgx::_charSet_node::addUDigitRange() {
    _delOPT |= NO_DIGIT;
}


string rgx::_charSet_node::toString() {
    string info; 
    info += " _charSet_node \n";
    info += " inversion : " + bool2string(inversion) + "\n";
    info += " charRange :\n";
    for (auto p : _acceptSet) {
        u16string ws;
        ws.push_back(' ');
        ws.push_back(p.first);
        ws.push_back(' ');
        ws.push_back(p.second - 1);
        ws.push_back('\n');
        info += ucs2_to_string(ws);
    }
    info += " deleteRange :";
    if (_delOPT & NO_WORD) {
        info += " NO_WORD_OPT ";
    } 
    if (_delOPT & NO_DIGIT) {
        info += " NO_DIGIT_OPT ";
    }
    if (_delOPT & NO_SPACE) {
        info += " NO_SPACE_OPT ";
    }
    info += "\n\n";
    return info; 
}

void rgx::_charSet_node::generateNFA() {
    cout << "charSet_node_generate_NFA\n" << endl;
    if (_left || _right) {
        err(" charSet出现孩子节点 ");
    }
    auto startNode = make_shared<_NFA_Node>();
    auto finishNode = make_shared<_NFA_Node>();
    auto newEdge = startNode->addCharSetEdge(finishNode);
    newEdge->_delOPT = _delOPT;
    if (auto p = _edgeMgr.lock()) {
        for (auto range : _acceptSet) {
            unsigned int pre = p->_hashTable[range.first];
            newEdge->_acceptSet.insert(pre);
            for (auto i = range.first; i <  range.second; ++i) {
                if (p->_hashTable[i] != pre) {
                    newEdge->_acceptSet.insert(pre);
                    pre = p->_hashTable[i];
                }
            }
        }
    } else {
        err("in function rgx::_charSet_node::generateNFA()  \n\n 找不到对象: _edgeMgr \n\n");
    }
    _nfa = make_shared<_NFA>(startNode, finishNode);
}


/*-----------------------------------------------*/

rgx::_preRead_node::_preRead_node(bool tag) : pattern_tag(tag), _dfaTree(nullptr) {

}

rgx::_preRead_node::_preRead_node(shared_ptr<_astNode> reTerm, bool tag) : pattern_tag(tag), _dfaTree(reTerm) {

} 


string rgx::_preRead_node::toString() {
    string info;
    info += " _preRead_node ";
    return info;
}


_DFA_ptr rgx::_preRead_node::generateDFA() {
    return nullptr;
}


/*-----------------------------------------------*/

rgx::_reference_node::_reference_node(unsigned int id) : _referenceIndex(id) {
    
}

rgx::_reference_node::_reference_node(unsigned int id, const u16string& n) : _referenceName(n), _referenceIndex(id) {

}

string rgx::_reference_node::toString() {
    string info;
    info += " _reference_node \n";
    info += " reference Index: " + int2string(_referenceIndex);
    info += "\n\n";
    return info;
}


void rgx::_reference_node::generateNFA() {
    cout << "reference_node_generate_NFA\n" << endl;
    if (_left || _right) {
        err(" referenceNode出现子节点 ");
    }
    auto startNode = make_shared<_NFA_Node>();
    auto finishNode = make_shared<_NFA_Node>();
    auto newEdge = startNode->addReferenceEdge(finishNode);
    newEdge->_referenceIndex = _referenceIndex;
    _nfa = make_shared<_NFA>(startNode, finishNode);
}


/*-----------------------------------------------*/

rgx::_numCount_node::_numCount_node() : _greedy(true) , _lowerLoopTimes(1), _upperLoopTimes(1) {

}

rgx::_numCount_node::_numCount_node(int l, int u) : _greedy(true), _lowerLoopTimes(l), _upperLoopTimes(u) {

}

rgx::_numCount_node::_numCount_node(int l, int u, bool g) : _greedy(g), _lowerLoopTimes(l), _upperLoopTimes(u) {

}

string rgx::_numCount_node::toString() {
   string info; 
   info += " _numCount_node \n";
   if (_pre_read) {
       info += " pre_read \n";
   }
   if (_post_read) {
       info += " post_read \n";
   }
   info += " greedy : " + bool2string(_greedy) + "\n";
   info += " range " + int2string(_lowerLoopTimes) + " " + int2string(_upperLoopTimes);
   info += "\n\n";
   return info;
}


void rgx::_numCount_node::generateNFA() {
    cout << "numCount_node_generate_NFA\n" << endl;
    if (!_left || (_left && !_left->_nfa) || _right) {
        cout << toString() << endl;
        err(" 生成NFA时出错 ");
    } 
    auto child = _left->_nfa;
    auto loopStart = child->first->addLoopStartEdge(child->second);
    auto loopEnd = child->second->addLoopEndEdge(child->first);
    loopStart->_lowerLoopTimes  = _lowerLoopTimes;
    loopStart->_upperLoopTimes = _upperLoopTimes;
    loopStart->_greedy = _greedy; 
    loopEnd->_lowerLoopTimes = _lowerLoopTimes;
    loopEnd->_upperLoopTimes = _upperLoopTimes;
    loopEnd->_greedy = _greedy;
    _nfa = make_shared<_NFA>(child->first, child->second);
}

/*-----------------------------------------------*/

string rgx::_capture_node::toString() {
    string info;
    info += " _capture_node \n";
    info += " captureIndex : " + int2string(_captureIndex) + "\n";
    info += " captureName : " + ucs2_to_string(_captureName);
    info += "\n\n";
    return info;
}

void rgx::_capture_node::generateNFA() {
    cout << "captureCount_node_generate_NFA\n" << endl;
    if (!_left || !_left->_nfa || _right) {
        cout << toString() << endl;
        err(" 生成NFA时出错 ");
    }
    auto startNode = make_shared<_NFA_Node>();
    auto finishNode = make_shared<_NFA_Node>();
    auto child = _left->_nfa;
    auto captureStart = startNode->addCaptureStartEdge(child->first);
    auto captureEnd = child->second->addCaptureEndEdge(finishNode);
    captureStart->_captureIndex = _captureIndex;
    captureEnd->_captureIndex = _captureIndex;
    _nfa = make_shared<_NFA>(startNode, finishNode);
}

/*-----------------------------------------------*/

string rgx::_cat_node::toString() {
    string info;
    info += " _cat_node \n\n";
    return info;
}

void rgx::_cat_node::generateNFA() {
    cout << "cat_node_generate_NFA\n" << endl;
    if (!_left || !_left->_nfa || !_right || !_right->_nfa) {
        cout << toString() << endl;
        err(" 生成NFA 时出错 ");
    }
    auto leftChild = _left->_nfa;
    auto rightChild = _right->_nfa;
    leftChild->second->addEpsilonEdge(rightChild->first);
    _nfa = make_shared<_NFA>(leftChild->first, rightChild->second);
}

/*-----------------------------------------------*/

rgx::_position_node::_position_node(position_type p) : _position(p) {
}

string rgx::_position_node::toString() {
    string info;
    info += " _position_node \n";
    info += " position : " + positionString();
    info += "\n\n";
    return info;
}

string rgx::_position_node::positionString() {
    switch (_position) {
        case LINE_BEGIN :
            return " LINE_BEGIN ";
        case LINE_END :
            return " LINE_END ";
        case STRING_BEGIN :
            return " STRING_BEGIN ";
        case STRING_END :
            return " STRING_END "; 
        case BREAK_OFF :
            return " BREAK_OFF ";
        case NO_BREAK_OFF :
            return " NO_BREAK_OFF ";
        default :
            return " TYPE ERROR !!! ";
    }
}

void rgx::_position_node::generateNFA() {
    cout << "position_node_generate_NFA\n" << endl;
    if (_left || _right) {
        cout << toString() << endl;
        err(" _position_node 出现字节点 ");
    }
    auto startNode = make_shared<_NFA_Node>();
    auto finishNode = make_shared<_NFA_Node>();
    auto newEdge = startNode->addPositionEdge(finishNode);
    newEdge->_position = _position;
    _nfa = make_shared<_NFA>(startNode, finishNode);
}
