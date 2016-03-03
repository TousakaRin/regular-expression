#include "thread.h"

using namespace std;
using namespace rgx;

rgx::_thread::_thread() {}

rgx::_thread::_thread(const visitor_ptr<_NFA_Node>&, const shared_ptr<_edgeManager>&, unsigned int) {

}


int match(std::stack<_thread>&, const std::unique_ptr<matchObj>&) {
    return 1;
}
