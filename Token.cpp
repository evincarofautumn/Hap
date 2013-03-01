#include "Token.h"

#include <ostream>

namespace hap {

std::ostream& operator<<(std::ostream& stream, const Token& token) {
  return stream << token.string;
}

}
