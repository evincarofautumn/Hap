#ifndef HAP_CONTEXT_H
#define HAP_CONTEXT_H

#include "Statement.h"

#include <map>
#include <memory>
#include <utility>
#include <vector>

namespace hap {

class Context {
public:
  enum Behavior {
    NORMAL = 0,
    REPEAT = 1,
    RESUME = 2,
  };
  void listen
    (std::shared_ptr<const Expression>,
     Behavior,
     std::shared_ptr<const Statement>,
     std::shared_ptr<Environment>);
private:
  struct Handler {
    Behavior behavior;
    std::shared_ptr<const Statement> handler;
    std::shared_ptr<Environment> environment;
  };
  std::map<std::shared_ptr<const Expression>, Handler> listeners;
};

}

#endif
