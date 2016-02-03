#ifndef _RGX_H_
#define _RGX_H_

#include <string> 
#include "pattern.h"
#include <memory>

namespace rgx {

/*
 *    usage :
 *    string regular_expression = u8"regular[123]"
 *    auto t = complie (regular_expression);
 *
 */


std::shared_ptr<_pattern> compile (const std::string& regular_expression);

void astTraversal(const _ast& ast);

}
#endif
