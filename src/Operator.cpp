#include "Operator.h"

#include <ostream>

using namespace std;

namespace hap {

ostream& operator<<(ostream& stream, const Operator& operator_) {
  return stream << operator_.operator_;
}

}
