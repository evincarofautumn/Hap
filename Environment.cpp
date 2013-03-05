#include "Environment.h"
#include "Expression.h"

#include <sstream>
#include <stdexcept>

using namespace std;

namespace hap {

void Environment::define(const string& name, unique_ptr<Value> value) {
  variables.insert(make_pair(name, move(value)));
}

const Value& Environment::operator[](const string& name) const {
  return *variables.find(name)->second;
}

Value& Environment::operator[](const string& name) {
  return *variables[name];
}

}
