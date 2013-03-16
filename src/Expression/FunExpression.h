#ifndef HAP_FUNEXPRESSION_H
#define HAP_FUNEXPRESSION_H

#include "Value.h"

#include "Environment.h"
#include "Statement.h"

#include <string>
#include <vector>

namespace hap {

class FunExpression : public Value {
public:
  FunExpression
    (const std::string&,
     const std::vector<std::string>&,
     std::shared_ptr<Statement>,
     Environment&);
  virtual Value::Type type() const final override {
    return Type::FUNCTION;
  }
  virtual FunExpression* copy() const final override {
    return new FunExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
  std::unique_ptr<Value> call
    (const std::vector<std::unique_ptr<Expression>>&) const;
private:
  FunExpression(const FunExpression&) = default;
  std::string identifier;
  std::vector<std::string> parameters;
  std::shared_ptr<Statement> body;
  mutable Environment environment;
};

}

#endif
