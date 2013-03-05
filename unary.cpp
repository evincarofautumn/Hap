#include "unary.h"
#include "Expression.h"

#include "unique_cast.h"

using namespace std;

namespace hap {

namespace unary {

unique_ptr<Value> identity
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  auto value(expression->eval(environment));
  value->assert_type(Value::INTEGER);
  return move(value);
}

unique_ptr<Value> negate
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  auto value(expression->eval(environment));
  value->assert_type(Value::INTEGER);
  unique_ptr<IntegerExpression> integer
    (static_unique_cast<IntegerExpression>(move(value)));
  integer->value = -integer->value;
  return static_unique_cast<Value>(move(integer));
}

unique_ptr<Value> val
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary val");
}

unique_ptr<Value> ref
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary ref");
}

unique_ptr<Value> not_
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary not");
}

unique_ptr<Value> lt
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary less than");
}

unique_ptr<Value> ge
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary greater than or equal to");
}

unique_ptr<Value> gt
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary greater than");
}

unique_ptr<Value> le
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary less than or equal to");
}

unique_ptr<Value> eq
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary equals");
}

unique_ptr<Value> ne
  (Environment& environment,
   const unique_ptr<const Expression>& expression) {
  throw runtime_error("unimplemented unary not equals");
}

}

}
