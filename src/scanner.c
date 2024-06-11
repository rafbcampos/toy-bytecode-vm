#include "scanner.h"
#include <string.h>

Scanner scanner;

void init_scanner(const char *source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

int is_at_end() { return *scanner.current == '\0'; }

Token make_token(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
}

static Token error_token(const char *message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;
  return token;
}

static char advance() {
  scanner.current++;
  return scanner.current[-1];
}

static int match(char expected) {
  if (is_at_end())
    return 0;
  if (*scanner.current != expected)
    return 0;
  scanner.current++;
  return 1;
}

static char peek() { return *scanner.current; }

static char peek_next() {
  if (is_at_end())
    return '\0';
  return scanner.current[1];
}

static void skip_whitespace() {
  for (;;) {
    char c = peek();
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance();
      break;
    case '\n':
      scanner.line++;
      advance();
      break;
    case '/':
      if (peek_next() == '/') {
        while (*scanner.current != '\n' && !is_at_end())
          advance();
      } else {
        return;
      }
      break;
    default:
      return;
    }
  }
}

Token string() {
  while (peek() != '"' && !is_at_end()) {
    if (peek() == '\n')
      scanner.line++;
    advance();
  }

  if (is_at_end())
    return error_token("Untermianted string.");

  advance();
  return make_token(TOKEN_STRING);
}

static int is_digit(char c) { return c >= '0' && c <= '9'; }

static int is_alpha(char c) {
  return (c >= 'a' && c < -'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static TokenType check_keyword(int start, int length, const char *rest,
                               TokenType type) {
  if (scanner.current - scanner.start == start + length &&
      memcmp(scanner.start + start, rest, length) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

// keywords trie
static TokenType identifier_type() {
  switch (scanner.start[0]) {
  case 'a':
    return check_keyword(1, 2, "nd", TOKEN_AND);
  case 'e':
    return check_keyword(1, 3, "lse", TOKEN_ELSE);
  case 'f':
    if (scanner.current - scanner.start > 1) {
      switch (scanner.start[1]) {
      case 'a':
        return check_keyword(2, 3, "lse", TOKEN_FALSE);
      case 'o':
        return check_keyword(2, 1, "r", TOKEN_FOR);
      case 'u':
        return check_keyword(2, 1, "n", TOKEN_FUN);
      }
    }
    break;
  case 'i':
    return check_keyword(1, 1, "f", TOKEN_IF);
  case 'n':
    return check_keyword(1, 2, "il", TOKEN_NIL);
  case 'o':
    return check_keyword(1, 1, "r", TOKEN_OR);
  case 'p':
    return check_keyword(1, 4, "rint", TOKEN_PRINT);
  case 'r':
    return check_keyword(1, 5, "eturn", TOKEN_RETURN);
  case 't':
    return check_keyword(1, 3, "rue", TOKEN_TRUE);
  case 'v':
    return check_keyword(1, 2, "ar", TOKEN_VAR);
  case 'w':
    return check_keyword(1, 4, "hile", TOKEN_WHILE);
  }
  return TOKEN_IDENTIFIER;
}

static Token identfier() {
  while (is_alpha(peek()) || is_digit(peek()))
    advance();
  return make_token(identifier_type());
}

Token scan_token() {
  skip_whitespace();
  scanner.start = scanner.current;

  if (is_at_end())
    return make_token(TOKEN_EOF);

  char c = advance();

  if (is_alpha(c)) {
    return identfier();
  }

  if (is_digit(c)) {
    while (is_digit(peek()))
      advance();

    if (peek() == '.' && is_digit(peek_next())) {
      advance();
      while (is_digit(peek()))
        advance();
    }

    return make_token(TOKEN_NUMBER);
  }

  switch (c) {
  case '(':
    return make_token(TOKEN_LEFT_PAREN);
  case ')':
    return make_token(TOKEN_RIGHT_PAREN);
  case '{':
    return make_token(TOKEN_LEFT_BRACE);
  case '}':
    return make_token(TOKEN_RIGHT_BRACE);
  case ';':
    return make_token(TOKEN_SEMICOLON);
  case ',':
    return make_token(TOKEN_COMMA);
  case '.':
    return make_token(TOKEN_DOT);
  case '-':
    return make_token(TOKEN_MINUS);
  case '+':
    return make_token(TOKEN_PLUS);
  case '/':
    return make_token(TOKEN_SLASH);
  case '*':
    return make_token(TOKEN_STAR);
  case '!':
    return make_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  case '=':
    return make_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '<':
    return make_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
  case '>':
    return make_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
  case '"':
    return string();
  }

  return error_token("Unexpected character.");
}
