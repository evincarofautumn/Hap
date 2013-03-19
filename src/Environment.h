#ifndef HAP_ENVIRONMENT_H
#define HAP_ENVIRONMENT_H

#include <map>
#include <memory>
#include <string>

namespace hap {

class Value;

class Environment {
public:
  Environment(Environment* = nullptr);
  void define(const std::string&, std::unique_ptr<Value>);
  Value& operator[](const std::string&);
private:
  Environment(const Environment&) = delete;
  Environment* parent;
  std::map<std::string, std::shared_ptr<Value>> variables;
};

}

#endif
