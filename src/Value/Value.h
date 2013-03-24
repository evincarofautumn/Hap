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
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const {
    return std::shared_ptr<Value>(copy());
  }
  virtual Value* copy() const = 0;
  virtual bool less(const Value&) const = 0;
  virtual Type type() const = 0;
};

template<Value::Type T>
struct value_traits {};

template<Value::Type T>
std::shared_ptr<typename value_traits<T>::type> eval_as
  (const std::shared_ptr<const Expression> expression,
   Context& context,
   const std::shared_ptr<Environment> environment) {
  const auto value(expression->eval(context, environment));
  value->assert_type(T);
  return std::static_pointer_cast<typename value_traits<T>::type>(value);
}

std::ostream& operator<<(std::ostream&, const Value::Type&);
bool operator<(const Value&, const Value&);

}

#endif
