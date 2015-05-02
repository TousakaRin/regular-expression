//
//  DFA.cpp
//  regular
//
//  Created by soultuanz on 15/5/2.
//  Copyright (c) 2015年 soultuanz. All rights reserved.
//

#include "DFA.h"
#include <algorithm>

//****************Dstates*******************//


states::states(treeNode::posSet *set)  {
    unmarked.insert(new treeNode::posSet(set->begin(), set->end()));
}

states::~states() {
    for (auto &w : marked){
        delete w;
    }
}

treeNode::posSet* states::exist(treeNode::posSet* set){
    for (auto w : unmarked){
        if (set->size() != w->size()) {
            continue;
        }
        if (equal(set->begin(), set->end(), w->begin())) {
            return w;
        }
    }
    for (auto w : marked){
        if (set->size() != w->size()) {
            continue;
        }
        if (equal(set->begin(), set->end(), w->begin())) {
            return w;
        }
    }
    return nullptr;
}


//********************DFA*************************//


DFA::DFA(treeNode::posSet* s) : start(s), index(0) {
}

void DFA::addPath(treeNode::posSet *s, int c, treeNode::posSet *u) {
    if (hash.count(s) == 0) {
        hash[s] = index++;
        dfa.push_back(std::vector<int>(128, -1));
    }
    if (hash.count(u) == 0) {
        hash[u] = index++;
        dfa.push_back(std::vector<int>(128, -1));
    }
    
    dfa[hash[s]][c] = hash[u];
}

void DFA::addAcceptState(treeNode::posSet *ps) {
    accept.insert(hash[ps]);
}

bool DFA::match(std::string &str){
    if (str.size() == 0) {
        return false;
    }
    int s = hash[start];
    int pos = 0;
    while (pos < str.size()) {
        s = dfa[s][str[pos++]];
        if (s == -1) {
            return false;
        }
    }
    if (accept.count(s) != 0)
        return true;
    else
        return false;
}
