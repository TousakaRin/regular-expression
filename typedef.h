#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_
    
namespace rgx {


enum position_type {LINE_BEGIN, LINE_END, STRING_BEGIN, STRING_END, BREAK_OFF, NO_BREAK_OFF}; //位置常量

enum deleteOPT {NO_WORD = 0x1, NO_DIGIT = 0x10, NO_SPACE = 0x100};  //删除字符集，\W, \D, \S

}
#endif

