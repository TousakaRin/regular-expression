#ifndef _EDGE_MANAGER_
#define _EDGE_MANAGER_

#include <array>
#include <iostream>

namespace rgx {

class _charSet_node;
class _NFA_Node;
class _edgeManager {
    friend _charSet_node;
    friend _NFA_Node;

public: 
    enum {_ENCODE_LENGTH = 65536};
    _edgeManager();
    void addRange(const std::pair<char16_t, char16_t>&);
    unsigned int getIndex(char16_t); 
    int statsCount();

private:
    std::array<unsigned int, _ENCODE_LENGTH> _hashTable;
    unsigned int _maxEdegs; 
};

}
#endif
