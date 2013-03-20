#include "unary.h"

#include "BooleanValue.h"
#include "IntegerValue.h"
#include "unique_cast.h"

#include <functional>

using namespace std;

namespace hap {

namespace unary {

shared_ptr<Value> identity
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  auto value(expression->eval(environment));
  value->assert_type(Value::INTEGER);
  return value;
}

shared_ptr<Value> negate
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  auto value(expression->eval(environment));
  value->assert_type(Value::INTEGER);
  shared_ptr<IntegerValue> integer
    (static_pointer_cast<IntegerValue>(value));
  integer->value = -integer->value;
  return static_pointer_cast<Value>(integer);
}

shared_ptr<Value> val
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary val");
}

shared_ptr<Value> ref
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary ref");
}

template<class F>
shared_ptr<Value> logical
  (F function,
   const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  auto a(expression->eval(environment));
  a->assert_type(Value::BOOLEAN);
  auto b(static_pointer_cast<BooleanValue>(a));
  return shared_ptr<Value>
    (new BooleanValue(function(b->value)));
}

shared_ptr<Value> not_
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  return logical(logical_not<bool>(), environment, expression);
}

shared_ptr<Value> lt
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary less than");
}

shared_ptr<Value> ge
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary greater than or equal to");
}

shared_ptr<Value> gt
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary greater than");
}

shared_ptr<Value> le
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary less than or equal to");
}

shared_ptr<Value> eq
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary equals");
}

shared_ptr<Value> ne
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary not equals");
}

}

}
