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
    info += " _astNode ";
    if (pre_read) {
        info += " pre_read ";
    }
    if (post_read) {
        info += " pos_resd ";
    }
    if (numCount) {
        info += string(" numCount ") + 
            " lower : "  + int2string(numCount->lower) + 
            " upper : " + int2string(numCount->upper) +
            " greedy : " + bool2string(numCount->greedy);
    } else {
        info += " errrrrrrror  no numCount found ";
    }
    return info;
}


/*-----------------------------------------------*/
string _or_node::toString() {
    string info;
    info += " or_node ";
    if (pre_read) {
        info += " pre_read ";
    }
    if (post_read) {
        info += " pos_resd ";
    }
    if (numCount) {
        info += string(" numCount ") + 
            " lower : "  + int2string(numCount->lower) + 
            " upper : " + int2string(numCount->upper) +
            " greedy : " + bool2string(numCount->greedy);
    } else {
        info += " errrrrrrror  no numCount found ";
    }
    return info;
}

/*-----------------------------------------------*/

_charSet_node::_charSet_node() : inversion(false) {

}

_charSet_node::_charSet_node(wchar_t c) : inversion(false) {
    addCharRange(pair<wchar_t, wchar_t>(c, c)); 
}


void _charSet_node::addCharRange(const pair<wchar_t, wchar_t>& cr) {
    charset.push_back(cr);
}

void _charSet_node::addCharRange(pair<wchar_t, wchar_t>&& cr) {
    //被声明为右值引用的东西既可以是左值引用也可以是右值引用，当它有名字时，是左值引用
    //否则为右值引用, 这里cr被当成右值传入函数，在addCharRange内部，变成了左值引用，
    //要实现完美转发至push_back需要使用std::move将cr变回右值
    charset.push_back(std::move(cr));
}

void _charSet_node::setInversison() {
    inversion = true;
}

/*-----------------------------------------------*/

_reference_node::_reference_node(unsigned int id) : index(id) {

}



/*-----------------------------------------------*/

_numCount_node::_numCount_node() : greedy(true) , lower(1), upper(1) {

}

_numCount_node::_numCount_node(int l, int u) : greedy(true), lower(l), upper(u) {

}

_numCount_node::_numCount_node(int l, int u, bool g) : greedy(g), lower(l), upper(u) {

}
