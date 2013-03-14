#ifndef HAP_BLOCKSTATEMENT_H
#define HAP_BLOCKSTATEMENT_H

#include "Statement.h"

namespace hap {

class BlockStatement : public Statement {
public:
  BlockStatement() {}
  void push(std::unique_ptr<Statement> statement) {
    statements.push_back(std::move(statement));
  }
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::vector<std::unique_ptr<Statement>> statements;
};

}

#endif
