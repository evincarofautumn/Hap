#include "binary.h"

#include "BooleanValue.h"
#include "Environment.h"
#include "IdentifierExpression.h"
#include "IntegerValue.h"
#include "unique_cast.h"

#include <functional>

using namespace std;

namespace hap {

namespace binary {

template<class F>
shared_ptr<Value> arithmetic
  (F function,
   Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  auto a(left->eval(context, environment));
  auto b(right->eval(context, environment));
  a->assert_type(Value::INTEGER);
  b->assert_type(Value::INTEGER);
  auto c(static_pointer_cast<IntegerValue>(a));
  auto d(static_pointer_cast<IntegerValue>(b));
  return shared_ptr<Value>
    (new IntegerValue(function(c->value, d->value)));
}

shared_ptr<Value> multiply
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic(multiplies<int>(), context, environment, left, right);
}

shared_ptr<Value> divide
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic(divides<int>(), context, environment, left, right);
}

shared_ptr<Value> modulate
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic(modulus<int>(), context, environment, left, right);
}

shared_ptr<Value> add
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic(plus<int>(), context, environment, left, right);
}

shared_ptr<Value> subtract
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  throw runtime_error("unimplemented -");
}

template<class T>
struct shifts_left {
  T operator()(T a, T b) {
    return a << b;
  }
};

shared_ptr<Value> shift_left
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic(shifts_left<int>(), context, environment, left, right);
}

template<class T>
struct shifts_right {
  T operator()(T a, T b) {
    return a >> b;
  }
};

shared_ptr<Value> shift_right
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic(shifts_right<int>(), context, environment, left, right);
}

template<class F>
shared_ptr<Value> relational
  (F function,
   Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  auto a(left->eval(context, environment));
  auto b(right->eval(context, environment));
  a->assert_type(Value::INTEGER);
  b->assert_type(Value::INTEGER);
  auto c(static_pointer_cast<IntegerValue>(a));
  auto d(static_pointer_cast<IntegerValue>(b));
  return shared_ptr<Value>
    (new BooleanValue(function(c->value, d->value)));
}

shared_ptr<Value> lt
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational(less<int>(), context, environment, left, right);
}

shared_ptr<Value> ge
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational(greater_equal<int>(), context, environment, left, right);
}

shared_ptr<Value> gt
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational(greater<int>(), context, environment, left, right);
}

shared_ptr<Value> le
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational(less_equal<int>(), context, environment, left, right);
}

shared_ptr<Value> eq
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational(equal_to<int>(), context, environment, left, right);
}

shared_ptr<Value> ne
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational(not_equal_to<int>(), context, environment, left, right);
}

shared_ptr<Value> and_
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  auto left_value(left->eval(context, environment));
  left_value->assert_type(Value::BOOLEAN);
  auto left_boolean(static_pointer_cast<BooleanValue>(left_value));
  if (!left_boolean->value)
    return shared_ptr<Value>(new BooleanValue(false));
  auto right_value(right->eval(context, environment));
  right_value->assert_type(Value::BOOLEAN);
  return right_value;
}

shared_ptr<Value> xor_
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  auto left_value(left->eval(context, environment));
  left_value->assert_type(Value::BOOLEAN);
  auto left_boolean(static_pointer_cast<BooleanValue>(left_value));
  auto right_value(right->eval(context, environment));
  right_value->assert_type(Value::BOOLEAN);
  auto right_boolean(static_pointer_cast<BooleanValue>(right_value));
  return shared_ptr<Value>
    (new BooleanValue(left_boolean->value != right_boolean->value));
}

shared_ptr<Value> or_
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  auto left_value(left->eval(context, environment));
  left_value->assert_type(Value::BOOLEAN);
  auto left_boolean(static_pointer_cast<BooleanValue>(left_value));
  if (left_boolean->value)
    return shared_ptr<Value>(new BooleanValue(true));
  auto right_value(right->eval(context, environment));
  right_value->assert_type(Value::BOOLEAN);
  return right_value;
}

shared_ptr<Value> assign
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  if (const auto left_identifier
      = dynamic_cast<const IdentifierExpression*>(left.get())) {
    auto value(right->eval(context, environment));
    (*environment)[left_identifier->identifier] = value;
    return value;
  }
  throw runtime_error("non-lvalue in assignment");
}

shared_ptr<Value> comma
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  throw runtime_error("unimplemented ,");
}

}

}
