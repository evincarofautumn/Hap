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
  ESCAPE,
};

const map<char, Token::Type> single_character_tokens {
  make_pair('(', Token::LEFT_PARENTHESIS),
  make_pair(')', Token::RIGHT_PARENTHESIS),
  make_pair(',', Token::COMMA),
  make_pair('.', Token::DOT),
  make_pair(':', Token::COLON),
  make_pair(';', Token::SEMICOLON),
  make_pair('[', Token::LEFT_BRACKET),
  make_pair(']', Token::RIGHT_BRACKET),
  make_pair('{', Token::LEFT_BRACE),
  make_pair('}', Token::RIGHT_BRACE),
};

vector<Token> tokenize(istream& stream) {
  vector<Token> tokens;
  string token;
  int character = 0;
  const string operator_characters = "!$%&*+-./<=>?@\\^|~";
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
            tokens.push_back
              (Token(found->second, string(1, character)));
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
      if (character == '_'
          || isalpha(character)
          || isdigit(character)) {
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
      switch (character) {
      case '"':
        token += character;
        tokens.push_back(Token(Token::STRING, token));
        state = NORMAL;
        break;
      case '\\':
        state = ESCAPE;
        break;
      default:
        token += character;
      }
      break;
    case ESCAPE:
      if (character == EOF)
        throw runtime_error("EOF in string escape.");
      switch (character) {
      case '\\':
        token += '\\';
        break;
      case '"':
        token += '"';
        break;
      case 'a':
        token += '\a';
        break;
      case 'b':
        token += '\b';
        break;
      case 'f':
        token += '\f';
        break;
      case 'n':
        token += '\n';
        break;
      case 'r':
        token += '\r';
        break;
      case 't':
        token += '\t';
        break;
      case 'v':
        token += '\v';
        break;
      default:
        {
          ostringstream message;
          message << "invalid escape '\\" << character << "'";
          throw runtime_error(message.str());
        }
      }
      state = STRING;
      break;
    }
  }
  return tokens;
}

}
