
#include "Context.h"

#include "Atomic.h"
#include "BooleanValue.h"

using namespace std;

namespace hap {

Context::Context() : atomic(false) {}

void Context::interrupt
  (const shared_ptr<Environment> environment) {
  if (atomic)
    return;
  vector<shared_ptr<const Statement>> statements;
  for (auto listener = listeners.begin();
       listener != listeners.end();
       ++listener) {
    if (dead(listener))
      continue;
    const auto& condition(listener->first);
    auto& handler(listener->second);
    if (handler.behavior & REPEAT)
      throw runtime_error("unimplemented repeat_*");
    bool value = false;
    {
      Atomic atomic(*this);
      value = eval_as<Value::BOOLEAN>
        (condition, *this, handler.environment)->value;
    }
    if (value) {
      if (handler.behavior & RESUME) {
        if (!handler.previous)
          statements.push_back(handler.statement);
      } else {
        statements.push_back(handler.statement);
        dead_listeners.push_back(listener);
      }
    }
    handler.previous = value;
  }
  for (const auto& dead_listener : dead_listeners)
    listeners.erase(dead_listener);
  dead_listeners.clear();
  for (const auto& statement : statements)
    statement->execute(*this, environment);
}

bool Context::dead(ListenerMap::iterator listener) const {
  return find(begin(dead_listeners), end(dead_listeners), listener)
    != dead_listeners.end();
}

void Context::listen
  (const shared_ptr<const Expression> condition,
   const Behavior behavior,
   const shared_ptr<const Statement> body,
   const shared_ptr<Environment> environment) {
  Handler handler{behavior, false, body, environment};
  listeners.insert(make_pair(condition, handler));
}

}
