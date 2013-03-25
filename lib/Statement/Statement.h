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
  void execute(Context&, std::shared_ptr<Environment>) const;
  friend bool operator==(const Statement&, const Statement&);
  friend std::ostream& operator<<(std::ostream&, const Statement&);
protected:
  Statement() {}
  virtual bool equal(const Statement&) const = 0;
  virtual void exec(Context&, std::shared_ptr<Environment>) const = 0;
  virtual void write(std::ostream&) const = 0;
private:
  Statement(const Statement&);
  Statement& operator=(const Statement&);
};

}

#endif
