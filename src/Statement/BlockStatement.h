#ifndef HAP_BLOCKSTATEMENT_H
#define HAP_BLOCKSTATEMENT_H

#include "Statement.h"

#include <vector>

namespace hap {

class BlockStatement : public Statement {
public:
  BlockStatement() {}
  void push(std::unique_ptr<Statement> statement) {
    statements.push_back(std::move(statement));
  }
  virtual void exec(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::vector<std::unique_ptr<Statement>> statements;
};

}

#endif
