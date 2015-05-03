//
//  treeNode.h
//  regular
//
//  Created by soultuanz on 15/4/27.
//  Copyright (c) 2015年 soultuanz. All rights reserved.
//

#ifndef __regular__treeNode__
#define __regular__treeNode__

#include <set>

class treeNode{
public:
    using posSet = std::set<treeNode*>;
    treeNode(char c);
    treeNode() { init_pointers(); }
    enum {CAT, OR, STAR, CHAR, END} type;
    void addFollows(treeNode*);
    
public:
    treeNode *left, *right;
    char value;
    posSet *first, *last, *follow;
    bool *isNullable;
    
private:
    void init_pointers();
public:
    ~treeNode();
};

#endif /* defined(__regular__treeNode__) */
