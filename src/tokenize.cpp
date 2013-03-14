#include "tokenize.h"

#include <cctype>
#include <istream>
#include <map>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace hap {

enum State {
  NORMAL,
  COMMENT,
  IDENTIFIER,
  INTEGER,
  OPERATOR,
  STRING,
};

vector<Token> tokenize(istream& stream) {
  vector<Token> tokens;
  string token;
  int character;
  const string operator_characters = "!$%&*+,-./:<=>?@\\^|~";
  map<char, Token::Type> single_character_tokens;
  {
    map<char, Token::Type>& _ = single_character_tokens;
    _['('] = Token::LEFT_PARENTHESIS;
    _[')'] = Token::RIGHT_PARENTHESIS;
    _['{'] = Token::LEFT_BRACE;
    _['}'] = Token::RIGHT_BRACE;
    _['['] = Token::LEFT_BRACKET;
    _[']'] = Token::RIGHT_BRACKET;
    _[','] = Token::COMMA;
    _[';'] = Token::SEMICOLON;
    _[':'] = Token::COLON;
  }
  State state = NORMAL;
  bool need = true;
  while (character != EOF) {
    if (need)
      character = stream.get();
    need = true;
    switch (state) {
    case NORMAL:
      switch (character) {
      case EOF:
        break;
      case '#':
        state = COMMENT;
        break;
      case '"':
        token = string(1, character);
        state = STRING;
        break;
      default:
        if (isspace(character))
          break;
        if (character == '_' || isalpha(character)) {
          token = string(1, character);
          state = IDENTIFIER;
          break;
        }
        if (isdigit(character)) {
          token = string(1, character);
          state = INTEGER;
          break;
        }
        {
          const map<char, Token::Type>::const_iterator found
            = single_character_tokens.find(character);
          if (found != single_character_tokens.end()) {
            tokens.push_back(Token(found->second, string(1, character)));
            break;
          }
        }
        if (operator_characters.find(character) != string::npos) {
          token = string(1, character);
          state = OPERATOR;
          break;
        }
        {
          ostringstream message;
          message
            << "Invalid character '"
            << character
            << "'.";
          throw runtime_error(message.str());
        }
      }
      break;
    case COMMENT:
      if (character == '\n' || character == EOF)
        state = NORMAL;
      break;
    case IDENTIFIER:
      if (character == '_' || isalpha(character) || isdigit(character)) {
        token += character;
      } else {
        tokens.push_back(Token(Token::IDENTIFIER, token));
        state = NORMAL;
        need = false;
      }
      break;
    case INTEGER:
      if (isdigit(character)) {
        token += character;
      } else {
        tokens.push_back(Token(Token::INTEGER, token));
        state = NORMAL;
        need = false;
      }
      break;
    case OPERATOR:
      if (operator_characters.find(character) != string::npos) {
        token += character;
      } else {
        tokens.push_back(Token(Token::OPERATOR, token));
        state = NORMAL;
        need = false;
      }
      break;
    case STRING:
      if (character == EOF)
        throw runtime_error("EOF in string.");
      token += character;
      if (character == '"') {
        tokens.push_back(Token(Token::STRING, token));
        state = NORMAL;
      }
      break;
    }
  }
  return tokens;
}

}
