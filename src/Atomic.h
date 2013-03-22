#ifndef HAP_ATOMIC_H
#define HAP_ATOMIC_H

namespace hap {

class Context;

class Atomic {
public:
  Atomic(Context&);
  ~Atomic();
private:
  Context& context;
  bool atomic;
};

}

#endif
