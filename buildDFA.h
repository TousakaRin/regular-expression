//
//  buildDFA.h
//  regular
//
//  Created by soultuanz on 15/5/1.
//  Copyright (c) 2015年 soultuanz. All rights reserved.
//

#ifndef __regular__buildDFA__
#define __regular__buildDFA__

#include "DFA.h"
#include "treeNode.h"

//extern class DFA;

DFA* buildDFA(treeNode*);
void init_first_and_last(treeNode*);
void init_follow(treeNode*);


#endif /* defined(__regular__buildDFA__) */
