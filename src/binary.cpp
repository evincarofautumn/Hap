#include "binary.h"

#include "BooleanValue.h"
#include "IntegerValue.h"
#include "unique_cast.h"

#include <functional>

using namespace std;

namespace hap {

namespace binary {

template<class F>
shared_ptr<Value> arithmetic
  (F function,
   const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  auto a(left->eval(environment));
  auto b(right->eval(environment));
  a->assert_type(Value::INTEGER);
  b->assert_type(Value::INTEGER);
  auto c(static_pointer_cast<IntegerValue>(a));
  auto d(static_pointer_cast<IntegerValue>(b));
  return shared_ptr<Value>
    (new IntegerValue(function(c->value, d->value)));
}

shared_ptr<Value> multiply
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(multiplies<int>(), environment, left, right);
}

shared_ptr<Value> divide
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(divides<int>(), environment, left, right);
}

shared_ptr<Value> modulate
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(modulus<int>(), environment, left, right);
}

shared_ptr<Value> add
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(plus<int>(), environment, left, right);
}

shared_ptr<Value> subtract
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  throw runtime_error("unimplemented -");
}

template<class T>
struct shifts_left {
  T operator()(T a, T b) {
    return a << b;
  }
};

shared_ptr<Value> shift_left
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(shifts_left<int>(), environment, left, right);
}

template<class T>
struct shifts_right {
  T operator()(T a, T b) {
    return a >> b;
  }
};

shared_ptr<Value> shift_right
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(shifts_right<int>(), environment, left, right);
}

template<class F>
shared_ptr<Value> relational
  (F function,
   const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  auto a(left->eval(environment));
  auto b(right->eval(environment));
  a->assert_type(Value::INTEGER);
  b->assert_type(Value::INTEGER);
  auto c(static_pointer_cast<IntegerValue>(a));
  auto d(static_pointer_cast<IntegerValue>(b));
  return shared_ptr<Value>
    (new BooleanValue(function(c->value, d->value)));
}

shared_ptr<Value> lt
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(less<int>(), environment, left, right);
}

shared_ptr<Value> ge
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(greater_equal<int>(), environment, left, right);
}

shared_ptr<Value> gt
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(greater<int>(), environment, left, right);
}

shared_ptr<Value> le
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(less_equal<int>(), environment, left, right);
}

shared_ptr<Value> eq
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(equal_to<int>(), environment, left, right);
}

shared_ptr<Value> ne
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(not_equal_to<int>(), environment, left, right);
}

template<class F>
shared_ptr<Value> logical
  (F function,
   const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
}

shared_ptr<Value> and_
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  auto left_value(left->eval(environment));
  left_value->assert_type(Value::BOOLEAN);
  auto left_boolean(static_pointer_cast<BooleanValue>(left_value));
  if (!left_boolean->value)
    return shared_ptr<Value>(new BooleanValue(false));
  auto right_value(right->eval(environment));
  right_value->assert_type(Value::BOOLEAN);
  return right_value;
}

shared_ptr<Value> xor_
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  auto left_value(left->eval(environment));
  left_value->assert_type(Value::BOOLEAN);
  auto left_boolean(static_pointer_cast<BooleanValue>(left_value));
  auto right_value(right->eval(environment));
  right_value->assert_type(Value::BOOLEAN);
  auto right_boolean(static_pointer_cast<BooleanValue>(right_value));
  return shared_ptr<Value>
    (new BooleanValue(left_boolean->value != right_boolean->value));
}

shared_ptr<Value> or_
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  auto left_value(left->eval(environment));
  left_value->assert_type(Value::BOOLEAN);
  auto left_boolean(static_pointer_cast<BooleanValue>(left_value));
  if (left_boolean->value)
    return shared_ptr<Value>(new BooleanValue(true));
  auto right_value(right->eval(environment));
  right_value->assert_type(Value::BOOLEAN);
  return right_value;
}

shared_ptr<Value> assign
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  throw runtime_error("unimplemented =");
}

shared_ptr<Value> comma
  (const std::shared_ptr<Environment> environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  throw runtime_error("unimplemented ,");
}

}

}
