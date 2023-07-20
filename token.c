#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char *value;
  enum { TOKEN_FUNC, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_ARGUMENTS, TOKEN_COMMA } type;
} token_T;

token_T *init_token(int type, char *value) {
  token_T *token = calloc(1, sizeof(token_T));
  token->type = type;
  token->value = value;

  return token;
}