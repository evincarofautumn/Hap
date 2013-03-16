#include "Parser.h"

#include <sstream>

using namespace std;

namespace hap {

void Parser::backtrack() {
  --current;
}

bool Parser::peek(Token::Type type) const {
  return !at_end() && current->type == type;
}

bool Parser::peek(const Token& token) const {
  return !at_end() && *current == token;
}

bool Parser::accept(Token::Type type) {
  if (!peek(type))
    return false;
  ++current;
  return true;
}

bool Parser::accept(Token::Type type, Token& result) {
  if (!peek(type))
    return false;
  result = *current++;
  return true;
}

bool Parser::accept(const Token& token) {
  if (!peek(token))
    return false;
  ++current;
  return true;
}

void Parser::expect(Token::Type type) {
  if (!accept(type))
    expected(type);
}

void Parser::expect(Token::Type type, Token& result) {
  if (!accept(type, result))
    expected(type);
}

void Parser::expect(const Token& token) {
  if (!accept(token))
    expected(token);
}

void Parser::expected(const string& abstract) const {
  ostringstream message;
  message << "expected " << abstract << " but got ";
  if (at_end()) {
    message << "end of input";
  } else {
    message << *current;
  }
  throw runtime_error(message.str());
}

void Parser::expected(Token::Type type) const {
  ostringstream message;
  message << "expected " << type << " but got ";
  if (at_end()) {
    message << "end of input";
  } else {
    message << *current;
  }
  throw runtime_error(message.str());
}

void Parser::expected(const Token& token) const {
  ostringstream message;
  message << "expected " << token << " but got ";
  if (at_end()) {
    message << "end of input";
  } else {
    message << *current;
  }
  throw runtime_error(message.str());
}

bool Parser::at_end() const {
  return current == tokens.end();
}

}
