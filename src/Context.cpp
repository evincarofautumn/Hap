#include "Context.h"

#include "BooleanValue.h"

using namespace std;

namespace hap {

void Context::interrupt
  (const shared_ptr<Environment> environment) {
  vector<shared_ptr<const Statement>> handlers;
  for (auto listener = listeners.begin();
       listener != listeners.end();
       ++listener) {
    const auto dead = find(begin(dead_listeners), end(dead_listeners), listener);
    if (dead != dead_listeners.end())
      continue;
    if (listener->second.behavior & REPEAT)
      throw runtime_error("unimplemented repeat_*");
    const auto value(listener->first->eval(*this, listener->second.environment));
    value->assert_type(Value::BOOLEAN);
    const auto condition(static_pointer_cast<BooleanValue>(value));
    if (!condition->value)
      continue;
    if (!(listener->second.behavior & RESUME))
      dead_listeners.push_back(listener);
    handlers.push_back(listener->second.handler);
  }
  for (const auto& dead_listener : dead_listeners)
    listeners.erase(dead_listener);
  dead_listeners.clear();
  for (const auto& handler : handlers)
    handler->execute(*this, environment);
}

void Context::listen
  (const shared_ptr<const Expression> condition,
   const Behavior behavior,
   const shared_ptr<const Statement> body,
   const shared_ptr<Environment> environment) {
  Handler handler{behavior, body, environment};
  listeners.insert(make_pair(condition, handler));
}

}
