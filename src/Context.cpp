#include "Context.h"

using namespace std;

namespace hap {

void Context::listen
  (const shared_ptr<const Expression> condition,
   const Behavior behavior,
   const shared_ptr<const Statement> body,
   const shared_ptr<Environment> environment) {
  Handler handler{behavior, body, environment};
  listeners.insert(make_pair(condition, handler));
}

}
