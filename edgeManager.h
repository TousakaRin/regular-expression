#ifndef _EDGE_MANAGER_
#define _EDGE_MANAGER_

#include <array>

namespace rgx {

class edgeManager {
public: 
    enum {_ENCODE_LENGTH = 65536};
    edgeManager();
    void addRange(const std::pair<char16_t, char16_t>&);
    unsigned int getIndex(char16_t); 
private:
    std::array<unsigned int, _ENCODE_LENGTH> _hashTable;
    unsigned int maxEdegs; 
};

}
#endif
