#ifndef HAP_TOKEN_H
#define HAP_TOKEN_H

#include <iosfwd>
#include <string>

namespace hap {

struct Token {
  enum Type {
    COMMA,
    IDENTIFIER,
    INTEGER,
    LEFT_BRACE,
    LEFT_BRACKET,
    LEFT_PARENTHESIS,
    OPERATOR,
    RIGHT_BRACE,
    RIGHT_BRACKET,
    RIGHT_PARENTHESIS,
    SEMICOLON,
    STRING,
  };
  Token(Type type, const std::string& string)
    : type(type), string(string) {}
  Type type;
  std::string string;
  friend std::ostream& operator<<(std::ostream&, const Token&);
};

}

#endif
