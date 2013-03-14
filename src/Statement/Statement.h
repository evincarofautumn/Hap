#ifndef HAP_STATEMENT_H
#define HAP_STATEMENT_H

#include "Environment.h"

#include <memory>
#include <string>
#include <vector>

namespace hap {

class Expression;

class Statement {
public:
  virtual ~Statement();
  virtual std::unique_ptr<Value> exec(Environment&) const = 0;
  virtual void write(std::ostream&) const = 0;
protected:
  Statement() {}
private:
  Statement(const Statement&);
  Statement& operator=(const Statement&);
};

std::ostream& operator<<(std::ostream&, const Statement&);

}

#endif
