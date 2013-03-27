#ifndef HAP_DOTEXPRESSION_H
#define HAP_DOTEXPRESSION_H

#include "Expression.h"

#include <string>

namespace hap {

class DotExpression : public Expression {
public:
  DotExpression(Expression*, const std::string&);
  DotExpression(std::shared_ptr<Expression>, const std::string&);
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::shared_ptr<Expression> expression;
  std::string key;
};

}

#endif
