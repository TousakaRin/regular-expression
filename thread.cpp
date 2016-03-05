#include "thread.h"
#include "nfaNode.h"

using namespace std;
using namespace rgx;

rgx::_thread::_thread(const visitor_ptr<_NFA_Node> &pc, unsigned int maxCaptureSlot, unsigned int sp, unsigned int edgeIndex) 
    :  _sp(sp), _nodePtr(pc), _edgeIndex(edgeIndex), _capture(new matchObj(maxCaptureSlot)), _maxCaptureSlot(maxCaptureSlot) { 
}

rgx::_thread::_thread(const visitor_ptr<_NFA_Node> &pc, unsigned int sp, unsigned int edgeIndex, const stack<unsigned int> &loopstack, unique_ptr<matchObj>&& capture)
    : _sp(sp), _nodePtr(pc), _edgeIndex(edgeIndex), _loopTimes(loopstack), _capture(std::move(capture)) {
        _maxCaptureSlot = _capture->_capVector.size();
}

rgx::_thread::_thread(const _thread& thread)
    : _sp(thread._sp), _nodePtr(thread._nodePtr), _edgeIndex(thread._edgeIndex), _loopTimes(thread._loopTimes), _capture(new matchObj(*thread._capture)), _maxCaptureSlot(thread._maxCaptureSlot) {

}

rgx::_thread::_thread(_thread&& thread)
    : _sp(thread._sp), _nodePtr(thread._nodePtr), _edgeIndex(thread._edgeIndex), _loopTimes(thread._loopTimes), _capture(std::move(thread._capture)), _maxCaptureSlot(thread._maxCaptureSlot) {

}

int rgx::_thread::match(const u16string& input, stack<_thread> &threadstack, const shared_ptr<_edgeManager> &edgeMgr) {
//    cout << "\n thread started..\n" << endl;
    while (_nodePtr->edges.size() != 0 && _nodePtr->edges[_edgeIndex]->match(input, *this, threadstack, edgeMgr) == 0) {
        ;
    }
 //   cout << "\n thread ended, edgesnumber :" << _nodePtr->edges.size() << endl;
    if (_nodePtr->edges.size() == 0) {
        //到达结束点
//        cout << "zhengquede jieguio" << endl;
//        _capture->justToTest("abcaabc");
        return 0;
    } else {
        return -1;
    }
}

void rgx::_thread::transTo(const visitor_ptr<_NFA_Node>& pc, stack<_thread>& threadstack) {
    //这里之所以可以按照任意顺序添加，因为在这里的NFA如果出现一个节点发出多条边
    //这些边是一定不存在贪婪关系的，因为循环的路径控制不在这里 
    //将线程状态移动至pc，若pc中有多条边，将其他的边加入线程栈
    _edgeIndex = 0;
    if (pc->edges.size() <= 1) {
        _nodePtr = pc;
    } else if (pc->edges.size() > 1) {
        _nodePtr = pc;
        for (unsigned int i = 1; i < pc->edges.size(); ++i) {
            threadstack.push(_thread(pc, _sp, i, _loopTimes, unique_ptr<matchObj>(new matchObj(*_capture))));
        }
    } 
}

