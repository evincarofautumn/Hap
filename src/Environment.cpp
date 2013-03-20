#include "Environment.h"
#include "UndefinedValue.h"

#include <sstream>
#include <stdexcept>

using namespace std;

namespace hap {

Environment::Environment(const weak_ptr<Environment> parent)
  : parent(parent) {}

void Environment::define(const string& name, shared_ptr<Value> value) {
  variables[name] = value;
}

shared_ptr<Value>& Environment::operator[](const string& name) {
  const auto existing(variables.find(name));
  if (existing == variables.end()) {
    if (const auto held_parent = parent.lock()) {
      return (*held_parent)[name];
    }
    variables[name].reset(new UndefinedValue());
    return variables[name];
  }
  return existing->second;
}

}
