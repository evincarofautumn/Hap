#ifndef HAP_TOKEN_H
#define HAP_TOKEN_H

#include <iosfwd>
#include <string>

namespace hap {

struct Token {
  enum Type {
    UNDEFINED,
    COLON,
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
  Token() : type(UNDEFINED) {}
  Token(Type type, const std::string& string)
    : type(type), string(string) {}
  Type type;
  std::string string;
  friend std::ostream& operator<<(std::ostream&, const Token&);
  friend bool operator==(const Token&, const Token&);
};

std::ostream& operator<<(std::ostream&, const Token::Type&);

}

#endif
