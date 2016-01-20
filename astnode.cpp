#include "astnode.h"
#include "stringTools.h"
#include <iostream>

using namespace std;
using namespace rgx;

/*-----------------------------------------------*/
_astNode::~_astNode() {
    //do nothing
}

string _astNode::toString() {
    string info;
    info += " astnode \n\n";
    return info;
}


/*-----------------------------------------------*/
string _or_node::toString() {
    string info;
    info += " or_node \n\n";
    return info;
}

/*-----------------------------------------------*/

_charSet_node::_charSet_node() : inversion(false) {

}

_charSet_node::_charSet_node(char16_t c) : inversion(false) {
    cout << "-------------------" << endl;
    cout << "err construction " << c << endl;
    cout << "-------------------" << endl;
}


void _charSet_node::addCharRange(const pair<char16_t, char16_t>& cr, shared_ptr<edgeManager> p) {
    p->getIndex('x');
    charset.push_back(cr);
}

void _charSet_node::addCharRange(pair<char16_t, char16_t>&& cr, shared_ptr<edgeManager> p) {
    //被声明为右值引用的东西既可以是左值引用也可以是右值引用，当它有名字时，是左值引用
    //否则为右值引用, 这里cr被当成右值传入函数，在addCharRange内部，变成了左值引用，
    //要实现完美转发至push_back需要使用std::move将cr变回右值
    p->getIndex('x');
    charset.push_back(std::move(cr));
}


void _charSet_node::addDeleteRange(const pair<char16_t, char16_t>& cr, shared_ptr<edgeManager> p) {
    p->getIndex('x');
    charset.push_back(cr);
}


void _charSet_node::addDeleteRange(pair<char16_t, char16_t>&& cr, shared_ptr<edgeManager> p) {
    p->getIndex('x');
    charset.push_back(std::move(cr));
}


void _charSet_node::setInversison() {
    inversion = true;
}


void _charSet_node::addWordRange(shared_ptr<edgeManager> p) {
    //左闭右开区间~~~~~
    addCharRange(pair<char16_t, char16_t>('a', 'z' + 1), p);
    addCharRange(pair<char16_t, char16_t>('A', 'Z' + 1), p);
    addCharRange(pair<char16_t, char16_t>('0', '9' + 1), p);
}

void _charSet_node::deleteWordRange(shared_ptr<edgeManager> p) {
    addDeleteRange(pair<char16_t, char16_t>('a', 'z' + 1), p);
    addDeleteRange(pair<char16_t, char16_t>('A', 'Z' + 1), p);
    addDeleteRange(pair<char16_t, char16_t>('0', '9' + 1), p);
}

void _charSet_node::addSpaceRang(shared_ptr<edgeManager> p) {
    addCharRange(pair<char16_t, char16_t>('\t', '\t' + 1), p);
    addCharRange(pair<char16_t, char16_t>('\r', '\r' + 1), p);
    addCharRange(pair<char16_t, char16_t>('\n', '\n' + 1), p);
    addCharRange(pair<char16_t, char16_t>('\b', '\b' + 1), p);
    addCharRange(pair<char16_t, char16_t>('\f', '\f' + 1), p);
    addCharRange(pair<char16_t, char16_t>('\v', '\v' + 1), p);
}

void _charSet_node::deleteSpaceRange(shared_ptr<edgeManager> p) {
    addDeleteRange(pair<char16_t, char16_t>('\t', '\t' + 1), p);
    addDeleteRange(pair<char16_t, char16_t>('\r', '\r' + 1), p);
    addDeleteRange(pair<char16_t, char16_t>('\n', '\n' + 1), p);
    addDeleteRange(pair<char16_t, char16_t>('\b', '\b' + 1), p);
    addDeleteRange(pair<char16_t, char16_t>('\f', '\f' + 1), p);
    addDeleteRange(pair<char16_t, char16_t>('\v', '\v' + 1), p);

}


void _charSet_node::addDigitRange(shared_ptr<edgeManager> p) {
    addCharRange(pair<char16_t, char16_t>('0', '9' + 1), p);
}

void _charSet_node::delteDigitRange(shared_ptr<edgeManager> p) {
    addDeleteRange(pair<char16_t, char16_t>('0', '9' + 1), p);
}


string _charSet_node::toString() {
    string info; 
    info += " _charSet_node \n";
    info += " inversion : " + bool2string(inversion) + "\n";
    for (auto p : charset) {
        u16string ws;
        ws.push_back(p.first);
        ws.push_back(' ');
        ws.push_back(p.second);
        ws.push_back('\n');
        info += ucs2_to_string(ws);
     }
     info += "\n";
     return info; 
}

/*-----------------------------------------------*/

_preRead_node::_preRead_node(bool tag) : pattern_tag(tag), dfaTree(nullptr) {

}

_preRead_node::_preRead_node(shared_ptr<_astNode> reTerm, bool tag) : pattern_tag(tag), dfaTree(reTerm) {

} 


string _preRead_node::toString() {
    string info;
    info += " _preRead_node ";
    return info;
}

/*-----------------------------------------------*/

_reference_node::_reference_node(unsigned int id) : index(id) {
    
}

_reference_node::_reference_node(unsigned int id, const u16string& n) : name(n), index(id) {

}

string _reference_node::toString() {
    string info;
    info += " _reference_node \n\n";
    return info;
}

/*-----------------------------------------------*/

_numCount_node::_numCount_node() : greedy(true) , lower(1), upper(1) {

}

_numCount_node::_numCount_node(int l, int u) : greedy(true), lower(l), upper(u) {

}

_numCount_node::_numCount_node(int l, int u, bool g) : greedy(g), lower(l), upper(u) {

}

string _numCount_node::toString() {
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



/*-----------------------------------------------*/

string _catch_node::toString() {
    string info;
    info += " _catch_node \n";
    info += " catchIndex : " + int2string(catchIndex) + "\n";
    info += " catchName : " + ucs2_to_string(name);
    info += "\n\n";
    return info;
}



/*-----------------------------------------------*/
string _cat_node::toString() {
    string info;
    info += " _cat_node \n\n";
    return info;
}



/*-----------------------------------------------*/
_position_node::_position_node(position_type p) : position(p) {
}

string _position_node::toString() {
    string info;
    info += " _position_node \n";
    info += " position : " + positionString();
    info += "\n\n";
    return info;
}

string _position_node::positionString() {
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
