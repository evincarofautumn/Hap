#include "unary.h"

#include "BooleanValue.h"
#include "IntegerValue.h"

#include <functional>

using namespace std;

namespace hap {

namespace unary {

#define UNARY_OPERATOR(NAME, IMPLEMENTATION) \
  make_pair(NAME, Operator \
    (Operator::UNARY, \
     NAME, \
     Operator::RIGHT, \
     Operator::TIGHTEST, \
     unary::IMPLEMENTATION)) \

map<string, Operator> operators {
  UNARY_OPERATOR("+", identity),
  UNARY_OPERATOR("-", negate),
  UNARY_OPERATOR("not", not_),
};

#undef UNARY_OPERATOR

shared_ptr<Value> identity
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& expression) {
  auto value(expression->eval(context, environment));
  value->assert_type(Value::INTEGER);
  return value;
}

shared_ptr<Value> negate
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& expression) {
  auto value(expression->eval(context, environment));
  value->assert_type(Value::INTEGER);
  shared_ptr<IntegerValue> integer
    (static_pointer_cast<IntegerValue>(value));
  integer->value = -integer->value;
  return static_pointer_cast<Value>(integer);
}

template<class F>
shared_ptr<Value> logical
  (F function,
   Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& expression) {
  auto a(expression->eval(context, environment));
  a->assert_type(Value::BOOLEAN);
  auto b(static_pointer_cast<BooleanValue>(a));
  return shared_ptr<Value>
    (new BooleanValue(function(b->value)));
}

shared_ptr<Value> not_
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& expression) {
  return logical(logical_not<bool>(), context, environment, expression);
}

}

}
