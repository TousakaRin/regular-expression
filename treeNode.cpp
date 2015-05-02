//
//  treeNode.cpp
//  regular
//
//  Created by soultuanz on 15/4/27.
//  Copyright (c) 2015年 soultuanz. All rights reserved.
//

#include "treeNode.h"

void treeNode::init_pointers(){
    first = nullptr;
    last = nullptr;
    follow = nullptr;
    isNullable = nullptr;
    left = nullptr;
    right = nullptr;
}

void treeNode::addFollows(treeNode* node){
    if (!(follow)){
        follow = new posSet();
    }
    follow->insert(node);
}

treeNode::~treeNode(){
    delete first;
    first = nullptr;
    
    delete last;
    last = nullptr;
    
    delete follow;
    follow = nullptr;
    
    delete isNullable;
    isNullable = nullptr;
}