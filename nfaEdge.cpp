#include "nfaEdge.h"
#include <string>
#include "stringTools.h"
#include "thread.h"
#include "typedef.h"
#include "nfaNode.h"

using namespace rgx;
using namespace std;

/*=========================_NFA_Edge=================================================*/

rgx::_NFA_Edge::_NFA_Edge(const visitor_ptr<_NFA_Node> &toNode) 
    : _toNode(toNode) {}

rgx::_NFA_Edge::_NFA_Edge(const _NFA_Edge& ne) 
    : _toNode(ne._toNode) {}

string rgx::_NFA_Edge::toString() {
    string info;
    info += "\n _NFA_Node \n";
    info += " an error occured \n";
    info += " this function should never be called \n\n";
    return info;
}

bool rgx::_NFA_Edge::lookahead(const u16string&, unsigned int) {
    return true;
}

/*================================epsilonEdge==========================================*/

rgx::_epsilonEdge::_epsilonEdge(const visitor_ptr<_NFA_Node> &toNode) 
    : _NFA_Edge(toNode) {}

rgx::_epsilonEdge::_epsilonEdge(const _epsilonEdge& ee) 
    : _NFA_Edge(ee) {}

_epsilonEdge* rgx::_epsilonEdge::makeCopy() const {
    return new _epsilonEdge(*this);   
}

string rgx::_epsilonEdge::toString() {
    string info;
    info += "\n epsilonEdge \n";
    return info;
}


int rgx::_epsilonEdge::match(const std::u16string&, _thread&, std::stack<_thread>&) {
    return -2;
}

/*===================================charSetEdge=======================================*/

rgx::_charSetEdge::_charSetEdge(const visitor_ptr<_NFA_Node> &toNode, set<unsigned int> &&s, const shared_ptr<_edgeManager>& edgeMgr, unsigned int delopt, bool ivs) 
    : _NFA_Edge(toNode), _acceptSet(s), _delOPT(delopt), _edgeMgr(edgeMgr), inversion(ivs) {}


_charSetEdge *rgx::_charSetEdge::makeCopy() const {
    return new _charSetEdge(*this);    
}

rgx::_charSetEdge::_charSetEdge(const _charSetEdge& cse) 
    : _NFA_Edge(cse._toNode), _acceptSet(cse._acceptSet), _delOPT(cse._delOPT), _edgeMgr(cse._edgeMgr), inversion(cse.inversion) {

}

string rgx::_charSetEdge::toString() {
    string info;
    info += "\n charSetEdge \n ";
    for (auto i : _acceptSet) {
        info += int2string(i) + " ";
    }
    info += "\n delete_OPT " + int2string(_delOPT);
    info += "\n";
    return info;
}


int rgx::_charSetEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>& threadstack) {
//    if (thread._sp < input.size())
//        cout << "\n charSetEdge " << thread._sp << " " << ucs2_to_string(input.substr(thread._sp, 1)) << "\n" << endl;
//    else
//        cout << "\n charSetEdge nononon \n" << endl;
    if (thread._sp >= input.size()) {
        return -1;
    }
    if (_delOPT & NO_WORD && (isDigit(input[thread._sp]) || isLetter(input[thread._sp]))) {
        //如果包含\W 标记并且sp指向数字或者字母，则匹配失败
        return -1;
    }
    if (_delOPT & NO_DIGIT && isDigit(input[thread._sp])) {
        //如果包含\D 且遇见数字，匹配失败
        return -1;
    }
    if (_delOPT & NO_SPACE && isSpace(input[thread._sp])) {
        //如果包含\S却遇见了空白字符
        return -1;
    }
//    if (inversion) {
//        cout << "inversion" << endl;
//    }
//    if (_acceptSet.find(_edgeMgr->getIndex(input[thread._sp])) == _acceptSet.end()) {
//        cout << "not find" << endl;
//    }
    if ((inversion == true) ^  (_acceptSet.find(_edgeMgr->getIndex(input[thread._sp])) == _acceptSet.end())) {
        return -1;
    } 
    ++thread._sp;
    thread.transTo(_toNode, input, threadstack);
    return 0;
}

bool rgx::_charSetEdge::lookahead(const u16string& input, unsigned int index) {
    if (_acceptSet.find(_edgeMgr->getIndex(input[index])) == _acceptSet.end()) {
        return false;
    } else {
        return true;
    }
}

/*================================loopStartEdge===========================================*/

rgx::_loopStartEdge::_loopStartEdge(const visitor_ptr<_NFA_Node> &toNode, const visitor_ptr<_NFA_Node> &loopEndNode, int llt, int ult, bool grd) 
    : _NFA_Edge(toNode), _lowerLoopTimes(llt), _upperLoopTimes(ult), _greedy(grd), _loopEndNode(loopEndNode) {}


rgx::_loopStartEdge::_loopStartEdge(const _loopStartEdge& lse)
    : _NFA_Edge(lse._toNode), _lowerLoopTimes(lse._lowerLoopTimes), _upperLoopTimes(lse._upperLoopTimes), _greedy(lse._greedy), _loopEndNode(lse._loopEndNode) {
    
}


string rgx::_loopStartEdge::toString() {
    string info;
    info += "\n loopStartEdge \n";
    info += " loopTimes : " + int2string(_lowerLoopTimes) + "  " + int2string(_upperLoopTimes) + "\n";
    info += " greedy : " + bool2string(_greedy) + "\n";
    return info;
}

_loopStartEdge* rgx::_loopStartEdge::makeCopy() const {
    return new _loopStartEdge(*this);
}

int rgx::_loopStartEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>& threadstack) {
//    cout << "_loopStartEdge\n\n" << endl;
    //此处应该preCheck先
    if (_upperLoopTimes == 0) {
        thread._loopTimes.push(0);
        thread.transTo(_loopEndNode, input, threadstack);
        return 0;
    } else if (_lowerLoopTimes > 0){
        thread._loopTimes.push(1);
        thread.transTo(_toNode, input, threadstack);
    } else {
        //  _lowerLoopTimes = 0 && _upperLoopTimes != 0, 此时在loopstartedge根据是否贪婪进行不同的split
        if (_greedy) {
            //将循环0次压栈,贪婪失败时尝试
            if (_loopEndNode->edges.size() == 0) {
                threadstack.push(_thread(_loopEndNode, thread._sp, 0, thread._loopTimes, unique_ptr<matchObj>(new matchObj(*thread._capture))));
            }
            for (unsigned int i = 0; i < _loopEndNode->edges.size(); ++i) {
                if (_loopEndNode->edges[i]->lookahead(input, thread._sp)) {
                    threadstack.push(_thread(_loopEndNode, thread._sp, i, thread._loopTimes, unique_ptr<matchObj>(new matchObj(*thread._capture))));
                }
            }
            //进入循环,记录循环次数
            thread._loopTimes.push(1);
            thread.transTo(_toNode, input, threadstack);
        } else {
            //将贪婪循环压栈
            for (unsigned int i = 0; i < _toNode->edges.size(); ++i) {
                if (_toNode->edges[i]->lookahead(input, thread._sp)) {
                    threadstack.push(_thread(_toNode, thread._sp, i, thread._loopTimes, unique_ptr<matchObj>(new matchObj(*thread._capture))));
                }
            }
            //优先执行非贪婪部分
            thread.transTo(_loopEndNode, input,  threadstack);
        }
    }
    return 0;
}


/*===================================loopEndEdge==========================================*/

rgx::_loopEndEdge::_loopEndEdge(const visitor_ptr<_NFA_Node> &toNode, const visitor_ptr<_NFA_Node>& loopStartNode, int llt, int ult, bool grd)
    : _NFA_Edge(toNode), _lowerLoopTimes(llt), _upperLoopTimes(ult), _greedy(grd) , _loopStartNode(loopStartNode) {}


rgx::_loopEndEdge::_loopEndEdge(const _loopEndEdge& lee) 
    : _NFA_Edge(lee._toNode), _lowerLoopTimes(lee._lowerLoopTimes), _upperLoopTimes(lee._upperLoopTimes), _greedy(lee._greedy), _loopStartNode(lee._loopStartNode)  {

}


string rgx::_loopEndEdge::toString() {
    string info;
    info += "\n loopEndEdge \n";
    info += " loopTimes : " + int2string(_lowerLoopTimes) + "  " + int2string(_upperLoopTimes) + "\n";
    info += " greedy : " + bool2string(_greedy) + "\n";
    return info;
}

_loopEndEdge* rgx::_loopEndEdge::makeCopy() const {
    return new  _loopEndEdge(*this);
}

int rgx::_loopEndEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>& threadstack) {
//    cout << "_loopEdge\n\n" << endl;
    if (thread._loopTimes.top() < static_cast<unsigned int>(_lowerLoopTimes)) {
        ++thread._loopTimes.top();
        thread.transTo(_loopStartNode, input, threadstack);
    } else if (_upperLoopTimes > 0 && thread._loopTimes.top() == static_cast<unsigned int>(_upperLoopTimes)) {
        //如果是最大次数，直接往后走
        thread._loopTimes.pop(); 
        thread.transTo(_toNode, input, threadstack);
    } else {
        //否则执行split
        if (_greedy) {
            //如果贪婪，优先往回走,并将往后走的thread入栈
            auto alreadyLoop = thread._loopTimes.top();
            thread._loopTimes.pop();
            if (_toNode->edges.size() == 0) {
                threadstack.push(_thread(_toNode, thread._sp, 0, thread._loopTimes, unique_ptr<matchObj>(new matchObj(*thread._capture))));
            }
            for (unsigned int edgeIndex = 0; edgeIndex < _toNode->edges.size(); ++edgeIndex) {
                if (_toNode->edges[edgeIndex]->lookahead(input, thread._sp)) {
                    threadstack.push(_thread(_toNode, thread._sp, edgeIndex, thread._loopTimes, unique_ptr<matchObj>(new matchObj(*thread._capture))));
                }
            }
            ++alreadyLoop;
            thread._loopTimes.push(alreadyLoop);
            thread.transTo(_loopStartNode, input, threadstack);
        } else {
            //否则优先往后走,并将循环的thread入栈
            ++thread._loopTimes.top();
            for (unsigned int i = 0; i < _loopStartNode->edges.size(); ++i) {
                if (_loopStartNode->edges[i]->lookahead(input, thread._sp)) {
                    threadstack.push(_thread(_loopStartNode, thread._sp, i, thread._loopTimes, unique_ptr<matchObj>(new matchObj(*thread._capture))));
                }
            }
            thread._loopTimes.pop();
            thread.transTo(_toNode, input, threadstack);
        }
    }
    return 0;
}

/*================================captureStart===========================================*/

rgx::_captureStartEdge::_captureStartEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int captureindex) 
    : _NFA_Edge(toNode), _captureIndex(captureindex) {}


rgx::_captureStartEdge::_captureStartEdge(const _captureStartEdge& cse) : _NFA_Edge(cse._toNode), _captureIndex(cse._captureIndex) {

}


string rgx::_captureStartEdge::toString() {
    string info;
    info += "\n captureStartEdge\n";
    return info;
}

_captureStartEdge* rgx::_captureStartEdge::makeCopy() const {
    return new _captureStartEdge(*this);
}

int rgx::_captureStartEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>& threadstack) {
//    cout << "_captureStartEdge  " << thread._sp <<"\n\n" << endl;
    thread._capture->_capVector[_captureIndex].first = thread._sp;
    thread.transTo(_toNode, input, threadstack);
    return 0;
}

bool rgx::_captureStartEdge::lookahead(const u16string& input, unsigned int index) {
    if (_toNode->edges.size() == 1) {
        return _toNode->edges[0]->lookahead(input, index);
    } else {
        return true;
    }
}

/*==============================captureEndEdge===========================================*/

rgx::_captureEndEdge::_captureEndEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int captureindex) 
    : _NFA_Edge(toNode), _captureIndex(captureindex) {}

rgx::_captureEndEdge::_captureEndEdge(const _captureEndEdge& cee) : _NFA_Edge(cee._toNode), _captureIndex(cee._captureIndex) {
    
}


string rgx::_captureEndEdge::toString() {
    string info;
    info += "\n captureEndEdge \n";
    info += " captureIndex : " + int2string(_captureIndex) + "\n";
    return info;
}

_captureEndEdge* rgx::_captureEndEdge::makeCopy() const {
    return new _captureEndEdge(*this);
}

int rgx::_captureEndEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>& threadstack) {
//    cout << "captureEdge : " << thread._sp << "\n\n" << endl;
    thread._capture->_capVector[_captureIndex].second = thread._sp;
    thread.transTo(_toNode, input, threadstack);
    return 0;
}

bool rgx::_captureEndEdge::lookahead(const u16string& input, unsigned int index) {
    if (_toNode->edges.size() == 1) {
        return _toNode->edges[0]->lookahead(input, index);
    } else {
        return true;
    }
}

/*===============================referenceEdge============================================*/

rgx::_referenceEdge::_referenceEdge(const visitor_ptr<_NFA_Node> &toNode, unsigned int referenceindex) 
    :_NFA_Edge(toNode), _referenceIndex(referenceindex) {}

rgx::_referenceEdge::_referenceEdge(const _referenceEdge& re) : _NFA_Edge(re._toNode), _referenceIndex(re._referenceIndex) {

}


string rgx::_referenceEdge::toString() {
    string info;
    info += "\n referenceEdge\n";
    info += " referenceIndex: " + int2string(_referenceIndex) + "\n\n";
    return info;
}

_referenceEdge* rgx::_referenceEdge::makeCopy() const {
    return new _referenceEdge(*this);
}
int rgx::_referenceEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>& threadstack) {
    for (unsigned int i = thread._capture->_capVector[_referenceIndex].first; i < thread._capture->_capVector[_referenceIndex].second; ) {
        if (input[i] == input[thread._sp]) {
            ++thread._sp;
            ++i;
        } else {
            return -1;
        }
    }
    thread.transTo(_toNode, input, threadstack);
    return 0;
}


/*================================positionEdge===========================================*/

rgx::_positionEdge::_positionEdge(const visitor_ptr<_NFA_Node> &toNode, position_type position) 
    : _NFA_Edge(toNode), _position(position) {}

rgx::_positionEdge::_positionEdge(const _positionEdge &pse) : _NFA_Edge(pse._toNode), _position(pse._position) {
    
}


string rgx::_positionEdge::toString() {
    string info;
    info += "\n positionEdge \n";
    info += " position_type : " + int2string(_position) + "\n\n";
    return info;
}

_positionEdge* rgx::_positionEdge::makeCopy() const {
    return new _positionEdge(*this);
}

int rgx::_positionEdge::match(const std::u16string& input, _thread& thread, std::stack<_thread>&) {
    switch (_position) {
        case LINE_BEGIN :
            if (thread._sp == 0 || input[thread._sp - 1] == '\n') {
                break; 
            } else {
                return -1;
            }
        case LINE_END :
            if (thread._sp == input.size() || input[thread._sp - 1] == '\n') {
                break;
            } else {
                return -1;
            }
        case STRING_BEGIN :
            if (thread._sp == 0) {
                break;
            } else {
                return -1;
            }
        case STRING_END :
            if (thread._sp == input.size()) {
                break;
            } else {
                return -1;
            }
        case BREAK_OFF :
            if (isWordBreak(input, thread._sp)) {
                break;
            } else {
                return -1;
            }
        case NO_BREAK_OFF :
            if (!isWordBreak(input, thread._sp)) {
                break;
            } else {
                return -1;
            }
        default : 
            return -1;
    } 
    thread._nodePtr = thread._nodePtr->edges[thread._edgeIndex]->_toNode;
    thread._edgeIndex = 0;
    return 0;
}

bool rgx::_positionEdge::isWordBreak(const u16string& input, unsigned int index) {
    if (index == 0 || index == input.size()) {
        return false;
    } else {
        if ((isDigit(input[index]) || isLetter(input[index])) && (!isDigit(input[index - 1]) && !isLetter(index - 1))) {
            return true;
        } else if ((isDigit(input[index - 1]) || isLetter(input[index - 1])) && (!isDigit(input[index]) && !isLetter(input[index]))) {
            return true;
        } else {
            return false;
        }
    }
}

