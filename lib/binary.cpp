#include "binary.h"

#include "BooleanValue.h"
#include "Environment.h"
#include "IdentifierExpression.h"
#include "IntegerValue.h"

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
  return shared_ptr<Value>
    (new IntegerValue
     (function
      (eval_as<Value::INTEGER>(left, context, environment)->value,
       eval_as<Value::INTEGER>(right, context, environment)->value)));
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
  return arithmetic(minus<int>(), context, environment, left, right);
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
  return shared_ptr<Value>
    (new BooleanValue
     (function
      (eval_as<Value::INTEGER>(left, context, environment)->value,
       eval_as<Value::INTEGER>(right, context, environment)->value)));
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
  return shared_ptr<Value>
    (new BooleanValue
     (eval_as<Value::BOOLEAN>(left, context, environment)->value
      && eval_as<Value::BOOLEAN>(right, context, environment)->value));
}

shared_ptr<Value> xor_
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return shared_ptr<Value>
    (new BooleanValue
     (eval_as<Value::BOOLEAN>(left, context, environment)->value
      != eval_as<Value::BOOLEAN>(right, context, environment)->value));
}

shared_ptr<Value> or_
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return shared_ptr<Value>
    (new BooleanValue
     (eval_as<Value::BOOLEAN>(left, context, environment)->value
      || eval_as<Value::BOOLEAN>(right, context, environment)->value));
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
