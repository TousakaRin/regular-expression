#ifndef _EDGE_MANAGER_
#define _EDGE_MANAGER_

#include <array>

namespace rgx {

class _charSet_node;
class _edgeManager {
    friend _charSet_node;

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
