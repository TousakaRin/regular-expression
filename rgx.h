#ifndef _RGX_H_
#define _RGX_H_

#include <string> 
#include "pattern.h"
#include <memory>

namespace rgx {

std::shared_ptr<pattern> compile (const std::wstring& regular_expression);


}
#endif
