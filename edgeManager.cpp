#include "edgeManager.h"

using namespace std;
using namespace rgx;

unsigned int rgx::edgeManager::getIndex(char16_t c) {
    return _hashTable[static_cast<size_t>(c)];
}


void rgx::edgeManager::addRange(const pair<char16_t, char16_t>& range) {
    size_t left = static_cast<size_t>(range.first), right = static_cast<size_t>(range.second);
    if (left == 0 || _hashTable[left - 1] != _hashTable[left]) {
        //left 未切开区间
        if (right == _ENCODE_LENGTH || _hashTable[right - 1] != _hashTable[right]) {
            //left和right都没有切开任何区间
            return;
        } else if (_hashTable[right - 1] == _hashTable[left]) {
            // left和right在同一个区间
            ++maxEdegs;
            for (size_t i = right; i != _ENCODE_LENGTH || _hashTable[i] == _hashTable[i - 1]; ++i) {
                _hashTable[i] = maxEdegs;
            }
        }
    } else {
        //left切开了区间
        if (right == _ENCODE_LENGTH || _hashTable[right] != _hashTable[right - 1]) {
            ++maxEdegs;
            for (size_t i = left; i != _ENCODE_LENGTH || _hashTable[i] == _hashTable[i - 1]; ++i) {
                _hashTable[i] = maxEdegs;
            }
        }
    }
}

rgx::edgeManager::edgeManager() : _hashTable() , maxEdegs(0) {
    for (size_t i = 0; i < _ENCODE_LENGTH; ++i) {
        _hashTable[i] = 0;
    } 
}
