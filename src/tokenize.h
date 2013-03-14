#ifndef HAP_TOKENIZE_H
#define HAP_TOKENIZE_H

#include "Token.h"

#include <iosfwd>
#include <vector>

namespace hap {

std::vector<Token> tokenize(std::istream&);

}

#endif
