#ifndef HAP_BLOCKSTATEMENT_H
#define HAP_BLOCKSTATEMENT_H

#include "Statement.h"

#include <vector>

namespace hap {

class BlockStatement : public Statement {
public:
  BlockStatement() {}
  void push(std::shared_ptr<Statement> statement) {
    statements.push_back(statement);
  }
protected:
  virtual bool equal(const Statement&) const;
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::vector<std::shared_ptr<Statement>> statements;
};

}

#endif
