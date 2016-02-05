#include "edgeManager.h"
#include <iostream>

using namespace std;
using namespace rgx;

unsigned int rgx::_edgeManager::getIndex(char16_t c) {
    return _hashTable[static_cast<size_t>(c)];
}


int rgx::_edgeManager::statsCount() {
    return _maxEdegs;    
}

void rgx::_edgeManager::addRange(const pair<char16_t, char16_t>& range) {
    int left = static_cast<int>(range.first), right = static_cast<int>(range.second);
    if (left != 0 && _hashTable[left - 1] == _hashTable[left]) {
        //当left切开了区间，将left-1至该区间起点的区间设置为新的区间
        ++_maxEdegs;
        auto bound = _hashTable[left];
        --left;
        while (left >= 0 && _hashTable[left] == bound) {
            _hashTable[left] = _maxEdegs;
            --left;
        }
    }

    if (right !=  _ENCODE_LENGTH && _hashTable[right] == _hashTable[right - 1]) {
        //当right切开了区间，将right到该区间终点的区间设置为新区间
        ++_maxEdegs;
        auto bound = _hashTable[right];
        while (right != _ENCODE_LENGTH && _hashTable[right] == bound) {
            _hashTable[right] = _maxEdegs;
            ++right;
        }
    }
}

rgx::_edgeManager::_edgeManager() :  _hashTable() , _maxEdegs(0) {
    for (size_t i = 0; i < _ENCODE_LENGTH; ++i) {
        _hashTable[i] = 0;
    } 
}
