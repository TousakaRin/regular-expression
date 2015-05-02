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


//static 全局变量定义
static int pos = 0;
static const std::string *regular = nullptr;
static char lookahead = ' ';
static std::set<char> keyword = {'|', '(', ')', '*'};

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
    endNode->first = new treeNode::posSet({endNode});
    endNode->last = new treeNode::posSet({endNode});
    endNode->isNullable = new bool(false);
    
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
        if (pos < regular->size() && keyword.count(lookahead) == 0) {
            treeNode *node = new treeNode();
            node->type = treeNode::CAT;
            node->left = root;
            node->right = TERM();
            root = node;
        }else{
            break;
        }
    }
    return  root;
}

treeNode *TERM(){
    treeNode *root = CHAR();
    if(pos < regular->size() && lookahead == '*'){
        match('*');
        treeNode *node = new treeNode();
        node->type = treeNode::STAR;
        node->left = root;
        node->isNullable = new bool(true);
        root = node;
    }
    return root;
}

treeNode *CHAR(){
    treeNode *root = nullptr;
    if(pos < regular->size() && lookahead == '('){
        match('(');
        root = OR();
        match(')');
    }else if(pos < regular->size()){
        if(lookahead == '\\' && pos < regular->size() - 1){
            lookahead = (*regular)[++pos];
        }else if(lookahead == '\\'){
            std::cout << "syntax error !" << std::endl;
        }
        root = new treeNode(lookahead);
        root->type = treeNode::CHAR;
        root->first = new treeNode::posSet({root});
        root->last = new treeNode::posSet({root});
        root->isNullable = new bool(false);
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