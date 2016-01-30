#include "astnode.h"
#include "stringTools.h"
#include "nfaEdge.h"
#include "nfaNode.h"
#include <iostream>

using namespace std;
using namespace rgx;



/*-----------------------------------------------*/
rgx::_astNode::~_astNode() {
    //do nothing
}

string rgx::_astNode::toString() {
    string info;
    info += " astnode \n\n";
    return info;
}

_NFA_ptr rgx::_astNode::generateNFA() {
    cout << "\nan error occured !!!!\ninformation:" << endl;
    cout << toString();
    return  nullptr;
}

void rgx::_astNode::err() {
    //出错时简单暴力的直接崩溃
    exit(-1);
}

void rgx::_astNode::err(const string& errMsg) {
    cout << "\n\n" << errMsg << endl;
    err();
}
/*-----------------------------------------------*/


string rgx::_or_node::toString() {
    string info;
    info += " or_node \n\n";
    return info;
}


_NFA_ptr rgx::_or_node::generateNFA() {
    // 首先新建开始与完成节点
    auto startNode = make_shared<_NFA_Node>(); 
    auto finishNode = make_shared<_NFA_Node>();

    //递归获得子节点的NFA
    auto leftChild = left->generateNFA();
    auto rightChild = right->generateNFA();

    //从startNode出发，新建两条epsilon边分别指向两个子节点NFA
    startNode->addEpsilonEdge(leftChild->first);
    startNode->addEpsilonEdge(rightChild->first);

    //将两个子节点的finishNode各新建一条epsilon边指向生成的finishNode
    leftChild->second->addEpsilonEdge(finishNode);
    rightChild->second->addEpsilonEdge(finishNode);

    return make_shared<_NFA>(startNode, finishNode); 
}

/*-----------------------------------------------*/

rgx::_charSet_node::_charSet_node(shared_ptr<edgeManager> edgemgr) : delOPT(0), edgeMgr(edgemgr), inversion(false) {

}

rgx::_charSet_node::_charSet_node(char16_t c) : inversion(false) {
    cout << "-------------------" << endl;
    cout << " err construction " << c << endl;
    cout << "-------------------" << endl;
}


void rgx::_charSet_node::addCharRange(const pair<char16_t, char16_t>& cr, shared_ptr<edgeManager> p) {
    p->addRange(cr);
    charset.push_back(cr);
}


void rgx::_charSet_node::setInversison() {
    inversion = true;
}


void rgx::_charSet_node::addWordRange() {
    //左闭右开区间~~~~~
    if (auto p = edgeMgr.lock()) {
        addCharRange(pair<char16_t, char16_t>('a', 'z' + 1), p);
        addCharRange(pair<char16_t, char16_t>('A', 'Z' + 1), p);
        addCharRange(pair<char16_t, char16_t>('0', '9' + 1), p);
    } else {
        err("in function rgx::_charSet_node::addWordRange()  \n\n 找不到对象: edgeMgr \n\n");
    }
}

void rgx::_charSet_node::addUWordRange() {
    delOPT |= NO_WORD;
}

void rgx::_charSet_node::addSpaceRang() {
    if (auto p = edgeMgr.lock()) {
        addCharRange(pair<char16_t, char16_t>('\t', '\t' + 1), p);
        addCharRange(pair<char16_t, char16_t>('\r', '\r' + 1), p);
        addCharRange(pair<char16_t, char16_t>('\n', '\n' + 1), p);
        addCharRange(pair<char16_t, char16_t>('\b', '\b' + 1), p);
        addCharRange(pair<char16_t, char16_t>('\f', '\f' + 1), p);
        addCharRange(pair<char16_t, char16_t>('\v', '\v' + 1), p);
    } else {
        err("in function rgx::_charSet_node::addSpaceRange()  \n\n 找不到对象: edgeMgr \n\n");
    }
}

void rgx::_charSet_node::addUSpaceRange() {
    delOPT |= NO_SPACE;
}


void rgx::_charSet_node::addDigitRange() {
    if (auto p = edgeMgr.lock()) {
        addCharRange(pair<char16_t, char16_t>('0', '9' + 1), p);
    } else {
        err("in function rgx::_charSet_node::addDigitRange()  \n\n 找不到对象: edgeMgr \n\n");
    }
}

void rgx::_charSet_node::addUDigitRange() {
    delOPT |= NO_DIGIT;
}


string rgx::_charSet_node::toString() {
    string info; 
    info += " _charSet_node \n";
    info += " inversion : " + bool2string(inversion) + "\n";
    info += " charRange :\n";
    for (auto p : charset) {
        u16string ws;
        ws.push_back(' ');
        ws.push_back(p.first);
        ws.push_back(' ');
        ws.push_back(p.second - 1);
        ws.push_back('\n');
        info += ucs2_to_string(ws);
    }
    info += " deleteRange :\n";
    if (delOPT & NO_WORD) {
        info += " NO_WORD_OPT ";
    } 
    if (delOPT & NO_DIGIT) {
        info += " NO_DIGIT_OPT ";
    }
    if (delOPT & NO_SPACE) {
        info += " NO_SPACE_OPT ";
    }
    info += "\n";
    return info; 
}

_NFA_ptr rgx::_charSet_node::generateNFA() {
    auto startNode = make_shared<_NFA_Node>();
    auto finishNode = make_shared<_NFA_Node>();
    auto newEdge = startNode->addCharSetEdge(finishNode);
    newEdge->delOPT = delOPT;
    if (auto p = edgeMgr.lock()) {
        unsigned int pre = p->_hashTable[0];
        newEdge->acceptSet.push_back(pre); 
        for (int i = 1; i < 65536; ++i) {
            if (p->_hashTable[i] != pre) {
                pre = p->_hashTable[i];
                newEdge->acceptSet.push_back(pre);
            }
        }
    } else {
        err("in function rgx::_charSet_node::generateNFA()  \n\n 找不到对象: edgeMgr \n\n");
    }
    return make_shared<_NFA>(startNode, finishNode);
}


/*-----------------------------------------------*/

rgx::_preRead_node::_preRead_node(bool tag) : pattern_tag(tag), dfaTree(nullptr) {

}

rgx::_preRead_node::_preRead_node(shared_ptr<_astNode> reTerm, bool tag) : pattern_tag(tag), dfaTree(reTerm) {

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

rgx::_reference_node::_reference_node(unsigned int id) : index(id) {
    
}

rgx::_reference_node::_reference_node(unsigned int id, const u16string& n) : name(n), index(id) {

}

string rgx::_reference_node::toString() {
    string info;
    info += " _reference_node \n\n";
    return info;
}


_NFA_ptr rgx::_reference_node::generateNFA() {
    auto startNode = make_shared<_NFA_Node>();
    auto finishNode = make_shared<_NFA_Node>();
    auto newEdge = startNode->addReferenceEdge(finishNode);
    newEdge->referenceIndex = index;
    return make_shared<_NFA>(startNode, finishNode);
}


/*-----------------------------------------------*/

rgx::_numCount_node::_numCount_node() : greedy(true) , lower(1), upper(1) {

}

rgx::_numCount_node::_numCount_node(int l, int u) : greedy(true), lower(l), upper(u) {

}

rgx::_numCount_node::_numCount_node(int l, int u, bool g) : greedy(g), lower(l), upper(u) {

}

string rgx::_numCount_node::toString() {
   string info; 
   info += " _numCount_node \n";
   if (pre_read) {
       info += " pre_read \n";
   }
   if (post_read) {
       info += " post_read \n";
   }
   info += " greedy : " + bool2string(greedy) + "\n";
   info += " range " + int2string(lower) + " " + int2string(upper);
   info += "\n\n";
   return info;
}


_NFA_ptr rgx::_numCount_node::generateNFA() {
    return nullptr;
}

/*-----------------------------------------------*/

string rgx::_capture_node::toString() {
    string info;
    info += " _capture_node \n";
    info += " captureIndex : " + int2string(captureIndex) + "\n";
    info += " captureName : " + ucs2_to_string(name);
    info += "\n\n";
    return info;
}

_NFA_ptr rgx::_capture_node::generateNFA() {
    auto startNode = make_shared<_NFA_Node>();
    auto finishNode = make_shared<_NFA_Node>();
    auto child = left->generateNFA();
    auto captureStart = startNode->addCaptureStartEdge(child->first);
    auto captureEnd = child->second->addCaptureEndEdge(finishNode);
    captureStart->referenceIndex = captureIndex;
    captureEnd->referenceIndex = captureIndex;
    return make_shared<_NFA>(startNode, finishNode);
}

/*-----------------------------------------------*/

string rgx::_cat_node::toString() {
    string info;
    info += " _cat_node \n\n";
    return info;
}

_NFA_ptr rgx::_cat_node::generateNFA() {
    auto leftChild = left->generateNFA();
    auto rightChild = right->generateNFA();
    leftChild->second->addEpsilonEdge(rightChild->first);
    return make_shared<_NFA>(leftChild->first, rightChild->second);
}

/*-----------------------------------------------*/

rgx::_position_node::_position_node(position_type p) : position(p) {
}

string rgx::_position_node::toString() {
    string info;
    info += " _position_node \n";
    info += " position : " + positionString();
    info += "\n\n";
    return info;
}

string rgx::_position_node::positionString() {
    switch (position) {
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

_NFA_ptr rgx::_position_node::generateNFA() {
    auto startNode = make_shared<_NFA_Node>();
    auto finishNode = make_shared<_NFA_Node>();
    auto newEdge = startNode->addPositionEdge(finishNode);
    newEdge->position = position;
    return make_shared<_NFA>(startNode, finishNode);
}
