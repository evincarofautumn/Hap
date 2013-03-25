#ifndef HAP_IDENTIFIEREXPRESSION_H
#define HAP_IDENTIFIEREXPRESSION_H

#include "Expression.h"

#include <string>

namespace hap {

class IdentifierExpression : public Expression {
public:
  IdentifierExpression(const std::string& identifier)
    : identifier(identifier) {}
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
  std::string identifier;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
};

}

#endif
