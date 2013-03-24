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
  friend class Atomic;
  enum Behavior {
    ONCE,
    REPEATEDLY,
  };
  Context();
  void interrupt(std::shared_ptr<Environment>);
  void listen
    (std::shared_ptr<const Expression>,
     Behavior,
     std::shared_ptr<const Statement>,
     std::shared_ptr<Environment>);
private:
  struct Handler {
    Behavior behavior;
    bool previous;
    std::shared_ptr<const Statement> statement;
    std::shared_ptr<Environment> environment;
  };
  typedef std::map<std::shared_ptr<const Expression>, Handler> ListenerMap;
  bool dead(ListenerMap::iterator) const;
  ListenerMap listeners;
  std::vector<ListenerMap::iterator> dead_listeners;
  bool atomic;
};

}

#endif
