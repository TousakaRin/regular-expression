//
//  buildTree.cpp
//  regular
//
//  Created by soultuanz on 15/4/27.
//  Copyright (c) 2015年 soultuanz. All rights reserved.
//

#include "buildTree.h"

#include <iostream>
#include <deque>
#include <set>
#include <sstream>
#include <string>

//#define _DEBUG_

//static 全局变量定义
static int pos = 0;
static const std::string *regular = nullptr;
static char lookahead = ' ';
static std::set<char> term = {'.'};
static std::set<char> cat = {'|', '*', '+', '{', '}', ')', ']'};

treeNode* buildTree(const std::string &input){
    regular = &input;
    lookahead = (*regular)[pos];
    treeNode *tree = OR();
    addEndSymbol(tree);
    return  tree;
}

void addEndSymbol(treeNode *&tree){
    //建立并初始化结束标记节点
    treeNode *endNode = new treeNode(0x80);
    endNode->type = treeNode::END;
    
    //将结束标记节点加入到语法树tree中，并使tree指向根节点
    treeNode *root = new treeNode();
    root->type = treeNode::CAT;
    root->left = tree;
    root->right = endNode;
    tree = root;
}

treeNode* OR(){
    treeNode *root = CAT();
    while (true) {
        if(lookahead == '|'){
            match('|');
            treeNode *node = new treeNode();
            node->type = treeNode::OR;
            node->left = root;
            node->right = CAT();
            root = node;
        }else{
            break;
        }
    }
    return root;
}

treeNode *CAT(){
    treeNode *root = TERM();
    while (true) {
        if (pos < regular->size() && cat.count(lookahead) == 0) {
            treeNode *right = TERM();
            if (right == nullptr) {     //如果term重复次数仅能为0
                continue;
            }
            treeNode *node = new treeNode();
            node->type = treeNode::CAT;
            node->left = root;
            node->right = right;
            root = node;
        }else{
            break;
        }
    }
    return  root;
}

treeNode *TERM(){
    treeNode *root = CHAR();
    
    
    //如果char有重复次数限定
    if(pos < regular->size() && lookahead == '*'){
        match('*');
        treeNode *node = new treeNode();
        node->type = treeNode::STAR;
        node->left = root;
        node->isNullable = new bool(true);    //这个地方应该封装进一个方法setNodeType
        root = node;
    }else if (pos < regular->size() && lookahead == '+') {
        match('+');
        treeNode *node = new treeNode();
        node->type = treeNode::CAT;
        node->left = root;
        
        treeNode *star = new treeNode();
        star->type = treeNode::STAR;
        star->left = treeCopy(root);
        star->isNullable = new bool(true);
        node->right = star;
        
        root = node;

    }else if (pos < regular->size() && lookahead == '{') {
        match('{');
        int low = 0;
        low = NUM();
        if (lookahead == '-') {
            match('-');
            int high = NUM();
            root = REPEAT(low, high, root);
        }else if(lookahead == '}') {
            match('}');
            root = REPEAT(low, low, root);
        }
    }
    return root;
}

treeNode *CHAR(){
    treeNode *root = nullptr;
    if(pos < regular->size() && lookahead == '('){
        match('(');
        root = OR();
        match(')');
    }else if(pos < regular->size() && lookahead == '[') {
        match('[');
        while (lookahead != ']') {
            treeNode *n = nullptr;
            if ((*regular)[pos + 1] == '-') {
                char start = lookahead;
                match(lookahead);
                match('-');
                char end = lookahead;
                match(lookahead);
                if (start > end) {    //出错提示
                    printError(pos - 3, pos);
                }
                n = new treeNode(start);
                for (char c = start+1; c <= end; ++c) {
                    treeNode *node = new treeNode();
                    node->type = treeNode::OR;
                    node->left = n;
                    node->right = new treeNode(c);
                    n = node;
                }
            }
            else {
                n = new treeNode(lookahead);    
                match(lookahead);
            }
            if (root == nullptr) {
                root = n;
                continue;
            }else {
                treeNode *node = new treeNode();
                node->type = treeNode::OR;
                node->left = root;
                node->right = n;
                root = node;
            }
        }
        match(']');
    }else if(pos < regular->size()){
        if(lookahead == '\\' && pos < regular->size() - 1){
            lookahead = (*regular)[++pos];
        }else if(lookahead == '\\'){
            std::cout << "syntax error !" << std::endl;
        }
        root = new treeNode(lookahead);
        match(lookahead);
    }
    return root;
}


void match(char c){
    if(pos >= regular->size()){
        std::cout << "到表达式末尾了还match, 尼玛这是出bug了啊" << std::endl;
        return;
    }
    if(lookahead == c){
        
#ifdef _DEBUG_
        std::cout << "match " << c << std::endl;
#endif
        
        ++pos;
        if(pos == regular->size()){
            std::cout << "GrammerTree building compeleted !" << std::endl;
        }else{
            lookahead = (*regular)[pos];
        }
    }else{
        std::cout << "syntax error !" << std::endl;
    }
}

//将一棵树重复low-high次得到的树
treeNode* REPEAT(int low, int high, treeNode* root){ ///////未完成///////
    if (low > high) {
        printError(pos-3, pos+2);
    }
    if (low == high) {
        
    }
    return  nullptr;
}


//复制一颗语法树
treeNode* treeCopy(treeNode *t) {
    treeNode *root = new treeNode();
    root->value = t->value;
    root->type = t->type;
    if (t->isNullable) {
        root->isNullable = new bool(*t->isNullable);
    }
    
    if (t->first) {
        root->first = new treeNode::posSet(t->first->begin(), t->first->end());
    }
    if (t->last) {
        root->last = new treeNode::posSet(t->last->begin(), t->last->end());
    }
    if (t->follow) {
        root->follow = new treeNode::posSet(t->follow->begin(), t->last->end());
    }
    if (t->left) {
        root->left = treeCopy(t->left);
    }
    if (t->right) {
        root->right = treeCopy(t->right);
    }
    return root;
}

//从类似{56-89}的语句中提取数字
int NUM() {
    std::string num;
    while (lookahead >= '0' && lookahead <= '9') {
        match(lookahead);
        num.push_back(lookahead);
    }
    std::stringstream ss(num);
    
    int i = 0;
    ss >> i;
    return  i;
}


void printError(int start, int end) {
    int low = start >= 0 ? start : 0;
    int high = end < regular->size() ? end : static_cast<int>(regular->size() - 1);
    std::cout << "写个正则都写错，死去吧你！" << std::endl;
    std::cout << "position : ...";
    for (; low <= high; ++low) {
        std::cout << (*regular)[low];
    }
    std::cout << "..." << std::endl;
}

void deleteTree(treeNode *root){
    if(root == nullptr){
        return;
    }
    std::deque<treeNode*> dq;
    dq.push_back(root);
    treeNode *node = nullptr;
    
#ifdef _DEBUG_
    int c = 0;
#endif
    
    while (!dq.empty()) {
        node = dq.front();
        dq.pop_front();
        if(node->left){
            dq.push_back(node->left);
        }
        if(node->right){
            dq.push_back(node->right);
        }
        delete node;
        
#ifdef _DEBUG_
        std::cout << c++ << std::endl;
#endif
    }
}