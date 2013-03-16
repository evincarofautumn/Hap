#ifndef HAP_IDENTIFIEREXPRESSION_H
#define HAP_IDENTIFIEREXPRESSION_H

#include "Expression.h"

#include <string>

namespace hap {

class IdentifierExpression : public Expression {
public:
  IdentifierExpression(const std::string& identifier)
    : identifier(identifier) {}
  virtual std::unique_ptr<Value> eval(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::string identifier;
};

}

#endif
