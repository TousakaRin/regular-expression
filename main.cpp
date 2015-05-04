//
//  main.cpp
//  regular
//
//  Created by soultuanz on 15/4/27.
//  Copyright (c) 2015年 soultuanz. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>

#include "buildTree.h"
#include "treeNode.h"
#include "buildDFA.h"


using namespace std;

void test(const char *, DFA*);

int main(int argc, const char * argv[]) {
       
    // insert code here...
    string input = "ab{0-3}d?a";
    cout << input << endl;
    treeNode *tree = buildTree(input);
    auto dfa = buildDFA(tree);
    deleteTree(tree);
    
    test("/Users/soultuanz/input", dfa);
    return 0;
}

void test (const char *input, DFA *dfa) {
    ifstream ifs(input);
    if (!ifs) {
        cout << "测试文件已经被吃掉。。。" << endl;
        return;
    }
    string str;
    while (ifs >> str) {
        if (dfa->match(str))
            cout << str << " matched !!" << endl;
    }
}