#include "Atomic.h"

#include "Context.h"

namespace hap {

Atomic::Atomic(Context& context) : context(context), atomic(context.atomic) {
  context.atomic = true;
}

Atomic::~Atomic() {
  context.atomic = atomic;
}

}
