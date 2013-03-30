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
  void undefine(const std::string&);
  std::shared_ptr<Value>& operator[](const std::string&);
  friend std::ostream& operator<<(std::ostream&, const Environment&);
private:
  Environment(const Environment&) = delete;
  std::weak_ptr<Environment> parent;
  std::map<std::string, std::shared_ptr<Value>> variables;
};

}

#endif
