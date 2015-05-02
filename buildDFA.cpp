//
//  buildDFA.cpp
//  regular
//
//  Created by soultuanz on 15/5/1.
//  Copyright (c) 2015年 soultuanz. All rights reserved.
//

#include "buildDFA.h"
#include "buildTree.h"


#include <deque>
#include <set>

using std::deque;
using std::set;

DFA *buildDFA(treeNode* root){
    //计算first，last，follow集
    init_first_and_last(root);
    init_follow(root);
    
    //构造DFA
    states Dstates(root->first);        //Dstates为自动变量，在函数结束时由析构函数释放堆内存
    //初始化DFA，将root->first置为start状态
    DFA *dfa = new DFA(root->first);
    
    
    
    while (!Dstates.unmarked.empty()) {
        auto S = *Dstates.unmarked.begin();
        Dstates.marked.insert(S);
        Dstates.unmarked.erase(Dstates.unmarked.begin());
        
        
        for (char c = 0; static_cast<unsigned>(c) < 128; ++c){  //对于每个可能的字符
            auto U = new treeNode::posSet();
            //将S中所有接受改字符的位置的follow集中位置加入U中
            for (auto w : *S){
                if (w->value == c) {
                    U->insert(w->follow->begin(), w->follow->end());
                }
            }

            auto p = Dstates.exist(U);    //exist(x)函数，若x在Dstates中，返回x，否则返回nullptr
            if (U->size() > 0 && !p){
                Dstates.unmarked.insert(U);
                p = U;
            }else if(U->size() > 0){   //若U在S中，则delete U
                delete U;
            }else{
                delete U;
                continue;
            }
            
            //增加一条转换move(S, c) = p
            //如果p中有end节点，将p加入到结束状态集
            //必须先addPath再addAcceptState，只有先addPath  dfa对象才会建立posSet* 到 DFA状态的映射
            dfa->addPath(S, c, p);
            for (auto w : *p) {
                if (w->type == treeNode::END){
                    dfa->addAcceptState(p);
                }
            }
            
        }
    }

    return dfa;
}


void init_first_and_last(treeNode* root){
    if (!(root->left->first && root->left->last && root->left->isNullable)){
        init_first_and_last(root->left);
    }
    if (root->type != treeNode::STAR && !(root->right->first && root->right->last && root->right-> isNullable)) {
        init_first_and_last(root->right);
    }
    if (root->type == treeNode::OR && !root->last && !root->first) {
        root->first = new treeNode::posSet();
        for (auto w : *root->left->first){
            root->first->insert(w);
        }
        for (auto w : *root->right->first){
            root->first->insert(w);
        }
        
        root->last = new treeNode::posSet();
        for (auto w : *root->right->last){
            root->last->insert(w);
        }
        for (auto w : *root->left->last){
            root->last->insert(w);
        }
        
        if (!(root->isNullable)) {
            root->isNullable = new bool();
            *root->isNullable = *root->left->isNullable || *root->right->isNullable;
        }
        return;
    }
    
    if (root->type == treeNode::CAT) {
        root->first = new treeNode::posSet();
        for (auto w : *root->left->first){
            root->first->insert(w);
        }
        if (*root->left->isNullable == true) {
            for (auto w : *root->right->first){
                root->first->insert(w);
            }
        }
        
        
        root->last = new treeNode::posSet();
        if (*root->right->isNullable == true) {
            for (auto w : *root->left->last){
                root->last->insert(w);
            }
        }
        for (auto w : *root->right->last){
            root->last->insert(w);
        }
        if (!(root->isNullable)) {
            root->isNullable = new bool();
            *root->isNullable = *root->left->isNullable && *root->right->isNullable;
        }
        return;
    }
    
    if (root->type == treeNode::STAR) {
        root->first = new treeNode::posSet(root->left->first->begin(), root->left->first->end());
        root->last = new treeNode::posSet(root->left->last->begin(), root->left->last->end());
        return;
    }
    
}

void init_follow(treeNode* root){
    deque<treeNode*> dq;
    dq.push_back(root);
    while (!dq.empty()) {
        treeNode *node = dq.front();
        dq.pop_front();
        if ((node->left)) {
            dq.push_back(node->left);
        }
        if ((node->right)) {
            dq.push_back(node->right);
        }
        if (node->type == treeNode::CAT) {
            for (auto w : *node->left->last){
                for (auto e : *node->right->first){
                    w->addFollows(e);
                }
            }
        }
        if (node->type == treeNode::STAR) {
            for (auto w : *node->last){
                for (auto e : *node->first){
                    w->addFollows(e);
                }
            }
        }
    }
}