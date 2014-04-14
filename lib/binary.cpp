#include "binary.h"

#include "BooleanValue.h"
#include "DotExpression.h"
#include "Environment.h"
#include "FloatValue.h"
#include "IdentifierExpression.h"
#include "IntegerValue.h"
#include "ListValue.h"
#include "MapValue.h"
#include "StringValue.h"
#include "SubscriptExpression.h"

#include <cmath>
#include <functional>
#include <sstream>

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
};

#undef BINARY_OPERATOR

template<class Function>
shared_ptr<Value> integer_arithmetic
  (Function function,
   Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return make_shared<IntegerValue>(function
    (eval_as<Value::INTEGER>(left, context, environment)->value,
     eval_as<Value::INTEGER>(right, context, environment)->value));
}

template
  <class FloatFunction,
   class IntegerFunction,
   class FloatType = FloatValue,
   class IntegerType = IntegerValue>
shared_ptr<Value> arithmetic
  (FloatFunction float_function,
   IntegerFunction integer_function,
   Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {

  const auto left_value(left->eval(context, environment));
  const auto right_value(right->eval(context, environment));
  const auto left_type(left_value->type());
  const auto right_type(right_value->type());

  const auto fail = [](Value::Type type) {
    ostringstream message;
    message << "expected integer or float but got " << type;
    throw runtime_error(message.str());
  };

  switch (left_type) {
  case Value::INTEGER:
    switch (right_type) {
    case Value::INTEGER:
      return make_shared<IntegerType>(integer_function
        (static_cast<const IntegerValue*>(left_value.get())->value,
         static_cast<const IntegerValue*>(right_value.get())->value));
    case Value::FLOAT:
      return make_shared<FloatType>(float_function
        (double(static_cast<const IntegerValue*>(left_value.get())->value),
         static_cast<const FloatValue*>(right_value.get())->value));
    default:
      fail(right_type);
    }

  case Value::FLOAT:
    switch (right_type) {
    case Value::INTEGER:
      return make_shared<FloatType>(float_function
        (static_cast<const FloatValue*>(left_value.get())->value,
         double(static_cast<const IntegerValue*>(right_value.get())->value)));
    case Value::FLOAT:
      return make_shared<FloatType>(float_function
        (static_cast<const FloatValue*>(left_value.get())->value,
         static_cast<const FloatValue*>(right_value.get())->value));
    default:
      fail(right_type);
    }

  default:
    fail(left_type);
  }
  throw runtime_error("impossible");
}

shared_ptr<Value> multiply
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic
    (multiplies<double>(), multiplies<int32_t>(),
     context, environment, left, right);
}

shared_ptr<Value> divide
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic
    (divides<double>(), divides<int32_t>(),
     context, environment, left, right);
}

shared_ptr<Value> modulate
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic
    (static_cast<double(*)(double, double)>(fmod),
     modulus<int32_t>(),
     context, environment, left, right);
}

shared_ptr<Value> add
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic
    (plus<double>(), plus<int32_t>(),
     context, environment, left, right);
}

shared_ptr<Value> subtract
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return arithmetic
    (minus<double>(), minus<int32_t>(),
     context, environment, left, right);
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
  return integer_arithmetic
    (shifts_left<int32_t>(), context, environment, left, right);
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
  return integer_arithmetic
    (shifts_right<int32_t>(), context, environment, left, right);
}

template
  <class FloatFunction,
   class IntegerFunction,
   class... Args>
shared_ptr<Value> relational
  (FloatFunction float_function,
   IntegerFunction integer_function,
   Args&&... args) {
  return arithmetic
    <FloatFunction, IntegerFunction, BooleanValue, BooleanValue>
    (float_function, integer_function, forward<Args>(args)...);
}

shared_ptr<Value> lt
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational
    (less<double>(), less<int>(),
     context, environment, left, right);
}

shared_ptr<Value> ge
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational
    (greater_equal<double>(), greater_equal<int>(),
     context, environment, left, right);
}

shared_ptr<Value> gt
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational
    (greater<double>(), greater<int>(),
     context, environment, left, right);
}

shared_ptr<Value> le
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational
    (less_equal<double>(), less_equal<int>(),
     context, environment, left, right);
}

shared_ptr<Value> eq
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational
    (equal_to<double>(), equal_to<int>(),
     context, environment, left, right);
}

shared_ptr<Value> ne
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return relational
    (not_equal_to<double>(), not_equal_to<int>(),
     context, environment, left, right);
}

shared_ptr<Value> and_
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return make_shared<BooleanValue>
    (eval_as<Value::BOOLEAN>(left, context, environment)->value
     && eval_as<Value::BOOLEAN>(right, context, environment)->value);
}

shared_ptr<Value> xor_
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return make_shared<BooleanValue>
    (eval_as<Value::BOOLEAN>(left, context, environment)->value
     != eval_as<Value::BOOLEAN>(right, context, environment)->value);
}

shared_ptr<Value> or_
  (Context& context,
   const shared_ptr<Environment> environment,
   const shared_ptr<const Expression>& left,
   const shared_ptr<const Expression>& right) {
  return make_shared<BooleanValue>
    (eval_as<Value::BOOLEAN>(left, context, environment)->value
     || eval_as<Value::BOOLEAN>(right, context, environment)->value);
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
  } else if (const auto dot
    = dynamic_cast<const DotExpression*>(left.get())) {
    const auto map(eval_as<Value::MAP>(dot->expression, context, environment));
    const auto value(right->eval(context, environment));
    (*map)[make_shared<StringValue>(dot->key)] = value;
    return value;
  } else if (const auto subscript
    = dynamic_cast<const SubscriptExpression*>(left.get())) {
    const auto container(subscript->expression->eval(context, environment));
    const auto key(subscript->subscript->eval(context, environment));
    const auto value(right->eval(context, environment));
    if (const auto list
      = dynamic_cast<ListValue*>(container.get())) {
      key->assert_type(Value::INTEGER);
      (*list)[static_cast<const IntegerValue*>(key.get())->value] = value;
    } else if (const auto map
      = dynamic_cast<MapValue*>(container.get())) {
      (*map)[key] = value;
    }
    return value;
  }
  throw runtime_error("non-lvalue in assignment");
}

}

}
