//
//  buildTree.h
//  regular
//
//  Created by soultuanz on 15/4/27.
//  Copyright (c) 2015年 soultuanz. All rights reserved.
//

#ifndef __regular__buildTree__
#define __regular__buildTree__

#include <stdio.h>
#include <string>
#include "treeNode.h"


// 建立语法树
treeNode *buildTree(const std::string&);
void addEndSymbol(treeNode*&);
treeNode *OR();
treeNode *TERM();
treeNode *CAT();
treeNode *CHAR();
treeNode *REPEAT(int, treeNode*); //将树重复0-i次，适应x{0-i}语法
int NUM();

//辅助函数
void match(char);
treeNode* treeCopy(treeNode*);

//析构语法树，释放内存
void deleteTree(treeNode*);

//出错提示
void printError(int, int);

//打印对应NFA中的重要位置，即DFA中接收字符的位置
void printImportantPos();
#endif /* defined(__regular__buildTree__) */
