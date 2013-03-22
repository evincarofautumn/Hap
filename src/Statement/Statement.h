#ifndef HAP_STATEMENT_H
#define HAP_STATEMENT_H

#include <memory>

namespace hap {

class Context;
class Environment;
class Expression;
class Value;

class Statement {
public:
  virtual ~Statement();
  virtual void exec(Context&, std::shared_ptr<Environment>) const = 0;
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
