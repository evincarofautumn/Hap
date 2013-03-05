#ifndef HAP_ENVIRONMENT_H
#define HAP_ENVIRONMENT_H

#include <memory>
#include <string>
#include <unordered_map>

namespace hap {

class Value;

class Environment {
public:
  void define(const std::string&, std::unique_ptr<Value>);
  const Value& operator[](const std::string&) const;
  Value& operator[](const std::string&);
private:
  std::unordered_map<std::string, std::unique_ptr<Value>> variables;
};

}

#endif
