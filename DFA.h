//
//  DFA.h
//  regular
//
//  Created by soultuanz on 15/5/2.
//  Copyright (c) 2015年 soultuanz. All rights reserved.
//

#ifndef __regular__DFA__
#define __regular__DFA__

#include "treeNode.h"
#include "buildTree.h"

#include <set>
#include <vector>
#include <map>

class states{
    
public:
    states(treeNode::posSet *set);
    ~states();
    treeNode::posSet* exist(treeNode::posSet*);
public:
    std::set<treeNode::posSet*> marked,unmarked;
};


class DFA {
public:
    DFA(treeNode::posSet*);
    void addPath(treeNode::posSet*, int, treeNode::posSet*);
    void addAcceptState(treeNode::posSet *);
    std::map<treeNode::posSet*, int> hash;
    bool match(std::string&);
    
    //DFA类通过位置集指针本身(即treeNode::posSet*)的值构造DFA中的状态，并不需要重新分配内存，内存由states类申请，释放
private:
    std::vector<std::vector<int>> dfa;
    int index;
    treeNode::posSet *start;
    std::set<int> accept;
};


#endif /* defined(__regular__DFA__) */
