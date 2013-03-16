#include "binary.h"

#include "BooleanValue.h"
#include "IntegerValue.h"
#include "unique_cast.h"

#include <functional>

using namespace std;

namespace hap {

namespace binary {

template<class F>
unique_ptr<Value> arithmetic
  (F function,
   Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  auto a(left->eval(environment));
  auto b(right->eval(environment));
  a->assert_type(Value::INTEGER);
  b->assert_type(Value::INTEGER);
  auto c(static_unique_cast<IntegerValue>(move(a)));
  auto d(static_unique_cast<IntegerValue>(move(b)));
  return unique_ptr<Value>
    (new IntegerValue(function(c->value, d->value)));
}

unique_ptr<Value> multiply
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(multiplies<int>(), environment, left, right);
}

unique_ptr<Value> divide
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(divides<int>(), environment, left, right);
}

unique_ptr<Value> modulate
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(modulus<int>(), environment, left, right);
}

unique_ptr<Value> add
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(plus<int>(), environment, left, right);
}

unique_ptr<Value> subtract
  (Environment& environment,
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

unique_ptr<Value> shift_left
  (Environment& environment,
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

unique_ptr<Value> shift_right
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return arithmetic(shifts_right<int>(), environment, left, right);
}

template<class F>
unique_ptr<Value> relational
  (F function,
   Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  auto a(left->eval(environment));
  auto b(right->eval(environment));
  a->assert_type(Value::INTEGER);
  b->assert_type(Value::INTEGER);
  auto c(static_unique_cast<IntegerValue>(move(a)));
  auto d(static_unique_cast<IntegerValue>(move(b)));
  return unique_ptr<Value>
    (new BooleanValue(function(c->value, d->value)));
}

unique_ptr<Value> lt
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(less<int>(), environment, left, right);
}

unique_ptr<Value> ge
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(greater_equal<int>(), environment, left, right);
}

unique_ptr<Value> gt
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(greater<int>(), environment, left, right);
}

unique_ptr<Value> le
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(less_equal<int>(), environment, left, right);
}

unique_ptr<Value> eq
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(equal_to<int>(), environment, left, right);
}

unique_ptr<Value> ne
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  return relational(not_equal_to<int>(), environment, left, right);
}

template<class F>
unique_ptr<Value> logical
  (F function,
   Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
}

unique_ptr<Value> and_
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  auto left_value(left->eval(environment));
  left_value->assert_type(Value::BOOLEAN);
  auto left_boolean(static_unique_cast<BooleanValue>(move(left_value)));
  if (!left_boolean->value)
    return unique_ptr<Value>(new BooleanValue(false));
  auto right_value(right->eval(environment));
  right_value->assert_type(Value::BOOLEAN);
  return move(right_value);
}

unique_ptr<Value> xor_
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  auto left_value(left->eval(environment));
  left_value->assert_type(Value::BOOLEAN);
  auto left_boolean(static_unique_cast<BooleanValue>(move(left_value)));
  auto right_value(right->eval(environment));
  right_value->assert_type(Value::BOOLEAN);
  auto right_boolean(static_unique_cast<BooleanValue>(move(right_value)));
  return unique_ptr<Value>
    (new BooleanValue(left_boolean->value != right_boolean->value));
}

unique_ptr<Value> or_
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  auto left_value(left->eval(environment));
  left_value->assert_type(Value::BOOLEAN);
  auto left_boolean(static_unique_cast<BooleanValue>(move(left_value)));
  if (left_boolean->value)
    return unique_ptr<Value>(new BooleanValue(true));
  auto right_value(right->eval(environment));
  right_value->assert_type(Value::BOOLEAN);
  return move(right_value);
}

unique_ptr<Value> assign
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  throw runtime_error("unimplemented =");
}

unique_ptr<Value> comma
  (Environment& environment,
   const unique_ptr<const Expression>& left,
   const unique_ptr<const Expression>& right) {
  throw runtime_error("unimplemented ,");
}

}

}
