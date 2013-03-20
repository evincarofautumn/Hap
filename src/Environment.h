#ifndef HAP_ENVIRONMENT_H
#define HAP_ENVIRONMENT_H

#include <map>
#include <memory>
#include <string>

namespace hap {

class Value;

class Environment {
public:
  Environment(std::weak_ptr<Environment> = std::weak_ptr<Environment>());
  void define(const std::string&, std::shared_ptr<Value>);
  std::shared_ptr<Value>& operator[](const std::string&);
private:
  Environment(const Environment&) = delete;
  std::weak_ptr<Environment> parent;
  std::map<std::string, std::shared_ptr<Value>> variables;
};

}

#endif
