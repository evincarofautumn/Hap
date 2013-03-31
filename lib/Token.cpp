#include "Token.h"

#include <ostream>
#include <stdexcept>

using namespace std;

namespace hap {

ostream& operator<<(ostream& stream, const Token& token) {
  return stream << token.type << " '" << token.string << "'";
}

ostream& operator<<(ostream& stream, const Token::Type& type) {
  switch (type) {
  case Token::UNDEFINED: throw runtime_error("undefined token");
  case Token::DOT: return stream << "dot";
  case Token::COLON: return stream << "colon";
  case Token::COMMA: return stream << "comma";
  case Token::IDENTIFIER: return stream << "identifier";
  case Token::INTEGER: return stream << "integer";
  case Token::FLOAT: return stream << "float";
  case Token::LEFT_BRACE: return stream << "left brace";
  case Token::LEFT_BRACKET: return stream << "left bracket";
  case Token::LEFT_PARENTHESIS: return stream << "left parenthesis";
  case Token::OPERATOR: return stream << "operator";
  case Token::RIGHT_BRACE: return stream << "right brace";
  case Token::RIGHT_BRACKET: return stream << "right bracket";
  case Token::RIGHT_PARENTHESIS: return stream << "right parenthesis";
  case Token::SEMICOLON: return stream << "semicolon";
  case Token::STRING: return stream << "string";
  }
}

bool operator==(const Token& a, const Token& b) {
  return a.type == b.type && a.string == b.string;
}

}
