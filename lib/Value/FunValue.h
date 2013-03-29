#ifndef HAP_FUNEXPRESSION_H
#define HAP_FUNEXPRESSION_H

#include "Value.h"

#include "Environment.h"
#include "Statement.h"

#include <string>
#include <vector>

namespace hap {

class FunValue : public Value {
public:
  FunValue
    (const std::string&,
     std::initializer_list<std::string>,
     Statement*);
  FunValue
    (const std::string&,
     const std::vector<std::string>&,
     std::shared_ptr<Statement>,
     std::shared_ptr<Environment>);
  virtual Value::Type type() const final override {
    return FUNCTION;
  }
  virtual FunValue* copy() const final override {
    return new FunValue(*this);
  }
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual bool less(const Value&) const final override;
  std::shared_ptr<Value> call
    (Context&, const std::vector<std::shared_ptr<Expression>>&) const;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  FunValue(const FunValue&) = default;
  std::string identifier;
  std::vector<std::string> parameters;
  std::shared_ptr<Statement> body;
  std::shared_ptr<Environment> environment;
};

template<>
struct value_traits<Value::FUNCTION> {
  typedef FunValue type;
};

}

#endif
