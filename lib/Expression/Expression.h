#ifndef HAP_EXPRESSION_H
#define HAP_EXPRESSION_H

#include <iosfwd>
#include <memory>

namespace hap {

class Context;
class Environment;
class Value;

class Expression {
public:
  virtual ~Expression();
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const = 0;
  virtual void write(std::ostream&) const = 0;
};

std::ostream& operator<<(std::ostream&, const Expression&);

}

#endif
