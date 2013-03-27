#include "binary.h"

#include "BooleanValue.h"
#include "Environment.h"
#include "IdentifierExpression.h"
#include "IntegerValue.h"
#include "ListValue.h"
#include "MapValue.h"
#include "StringValue.h"
#include "SubscriptExpression.h"

#include <functional>

using namespace std;

namespace hap {

namespace binary {

#define BINARY_OPERATOR(NAME, ASSOCIATIVITY, PRECEDENCE, IMPLEMENTATION) \
  make_pair(NAME, Operator \
    (Operator::BINARY, \
     NAME, \
     Operator::ASSOCIATIVITY, \
     Operator::PRECEDENCE, \
     binary::IMPLEMENTATION))

map<string, Operator> operators {
  BINARY_OPERATOR("*", LEFT, MULTIPLICATIVE, multiply),
  BINARY_OPERATOR("/", LEFT, MULTIPLICATIVE, divide),
  BINARY_OPERATOR("mod", LEFT, MULTIPLICATIVE, modulate),
  BINARY_OPERATOR("+", LEFT, ADDITIVE, add),
  BINARY_OPERATOR("-", LEFT, ADDITIVE, subtract),
  BINARY_OPERATOR("<<", LEFT, SHIFTING, shift_left),
  BINARY_OPERATOR(">>", LEFT, SHIFTING, shift_right),
  BINARY_OPERATOR("<", LEFT, RELATIONAL, lt),
  BINARY_OPERATOR(">=", LEFT, RELATIONAL, ge),
  BINARY_OPERATOR(">", LEFT, RELATIONAL, gt),
  BINARY_OPERATOR("<=", LEFT, RELATIONAL, le),
  BINARY_OPERATOR("==", LEFT, RELATIONAL, eq),
  BINARY_OPERATOR("<>", LEFT, RELATIONAL, ne),
  BINARY_OPERATOR("and", LEFT, AND, and_),
  BINARY_OPERATOR("xor", LEFT, XOR, xor_),
  BINARY_OPERATOR("or", LEFT, OR, or_),
  BINARY_OPERATOR("=", RIGHT, ASSIGNMENT, assign),
  BINARY_OPERATOR(",", RIGHT, COMMA, comma),
};

#undef BINARY_OPERATOR

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
    const auto value(right->eval(context, environment));
    (*environment)[left_identifier->identifier] = value;
    return value;
  } else if (const auto subscript
    = dynamic_cast<const SubscriptExpression*>(left.get())) {
    const auto value(right->eval(context, environment));
    const auto list
      (eval_as<Value::LIST>
       (subscript->expression, context, environment));
    const auto index
      (eval_as<Value::INTEGER>
       (subscript->subscript, context, environment));
    (*list)[index->value] = value;
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
