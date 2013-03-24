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
  virtual void write(std::ostream&) const = 0;
  void execute(Context&, std::shared_ptr<Environment>) const;
protected:
  Statement() {}
  virtual void exec(Context&, std::shared_ptr<Environment>) const = 0;
private:
  Statement(const Statement&);
  Statement& operator=(const Statement&);
};

std::ostream& operator<<(std::ostream&, const Statement&);

}

#endif
