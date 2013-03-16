#ifndef HAP_VALUE_H
#define HAP_VALUE_H

#include "Expression.h"

namespace hap {

class Value : public Expression {
public:
  enum Type {
    UNDEFINED,
    BOOLEAN,
    FUNCTION,
    INTEGER,
    LIST,
    MAP,
    STRING,
  };
  void assert_type(Type) const;
  virtual ~Value();
  virtual std::unique_ptr<Value> eval(Environment&) const {
    return std::unique_ptr<Value>(copy());
  }
  virtual Value* copy() const = 0;
  virtual Type type() const = 0;
};

std::ostream& operator<<(std::ostream&, const Value::Type&);

}

#endif