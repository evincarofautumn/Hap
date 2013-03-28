#ifndef HAP_FORSTATEMENT_H
#define HAP_FORSTATEMENT_H

#include "Statement.h"

namespace hap {

class ForStatement : public Statement {
public:
  ForStatement(Statement*, Expression*, Expression*, Statement*);
  ForStatement
    (std::shared_ptr<Statement>,
     std::shared_ptr<Expression>,
     std::shared_ptr<Expression>,
     std::shared_ptr<Statement>);
protected:
  virtual bool equal(const Statement&) const;
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::shared_ptr<Statement> initializer;
  std::shared_ptr<Expression> condition;
  std::shared_ptr<Expression> step;
  std::shared_ptr<Statement> body;
};

}

#endif
