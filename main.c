#include "array.c"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

Error *e_t;

int isNumber(const char *s)
{
  if (s == NULL || *s == '\0')
    return false;

  bool hasDigit = false;
  bool hasDecimalPoint = false;
  bool hasExponent = false;
  bool hasSign = false;

  while (*s)
  {
    if (*s == '-' || *s == '+')
    {
      if (hasSign || hasDigit || hasDecimalPoint || hasExponent)
        return false; // Sign not allowed in the middle
      hasSign = true;
    }
    else if (*s == '.')
    {
      if (hasDecimalPoint || hasExponent)
        return false; // Multiple decimal points or already in exponent
      hasDecimalPoint = true;
    }
    else if (*s == 'e' || *s == 'E')
    {
      if (hasExponent || !hasDigit)
        return false; // Multiple exponents or no digits before
      hasExponent = true;
      hasSign = false; // Reset sign for the exponent
    }
    else if (*s >= '0' && *s <= '9')
    {
      hasDigit = true;
    }
    else
    {
      return false; // Invalid character found
    }
    s++;
  }

  // Must have at least one digit to be considered a number
  return hasDigit;
}
double logbase(double y, int b)
{
  double lg;
  lg = log10(b) / log10(y);
  return (lg);
}

typedef struct ASTNode
{
  char *value;
  double arg_value;
  struct ASTNode *children[2];
  token_T *token;
  Error *e_t;
} ASTNode;
ASTNode *createNode(char *value, token_T *token);
ASTNode *parseExpression(TokenArray *tArray, int count, int index);
ASTNode *createAST(TokenArray *tArray, int count);

ASTNode *createNode(char *value, token_T *token)
{
  ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
  node->value = value;
  node->children[0] = NULL;
  node->children[1] = NULL;
  node->token = token;
  return node;
}

ASTNode *createAST(TokenArray *tArray, int count)
{
  int index = 0;
  return parseExpression(tArray, count, index);
}

int globalIndex = 0;

ASTNode *parseExpression(TokenArray *tArray, int count, int index)
{

  token_T token = *(tArray->tokens[index]);

  if (token.type == TOKEN_FUNC)
  {

    index++;
    if (index < count && tArray->tokens[index]->type == TOKEN_LPAREN)
    {
      index++;
      int localIndex = index;
      int parenCount = 1;
      int left = index;
      int right = -1;
      while (localIndex < count)
      {
        if (tArray->tokens[localIndex]->type == TOKEN_LPAREN)
        {
          parenCount++;
        }
        else if (tArray->tokens[localIndex]->type == TOKEN_RPAREN)
        {
          parenCount--;
        }

        if (parenCount == 0)
        {
          right = localIndex;
          break;
        }
        localIndex++;
      }
      if (right == -1)
      {
        e_t->index = localIndex;
        e_t->isError = true;
        e_t->type = "Unmatched parentheses";

        // fprintf(stderr, "Unmatched parentheses\n");
        // exit(1);
      }
      else
      {

        ASTNode *node = createNode(tArray->tokens[index - 2]->value,
                                   tArray->tokens[index - 2]);
        int gIndex = left;
        node->children[0] = parseExpression(tArray, right, left);
        int gLIndex = left;
        int parentCount = 0;
        int found = -1;
        while (gLIndex < right)
        {
          if (tArray->tokens[gLIndex]->type == TOKEN_LPAREN)
          {
            parenCount++;
          }
          else if (tArray->tokens[gLIndex]->type == TOKEN_RPAREN)
          {
            parenCount--;
          }
          if (parenCount == 0 && tArray->tokens[gLIndex]->type == TOKEN_COMMA)
          {
            found = 1;
            break;
          }

          gLIndex++;
        }
        if (found == 1)
        {
          node->children[1] = parseExpression(tArray, right, gLIndex + 1);
        }
        globalIndex = right;
        return node;
      }
    }
    else
    {
      e_t->index = index;
      e_t->isError = true;
      e_t->type = "Need atleat one argument";
      // fprintf(stderr, "Need atleat one argument \n");
      // exit(1);
    }
  }
  else
  {
    if (token.type == TOKEN_ARGUMENTS)
    {
      ASTNode *node =
          createNode(tArray->tokens[index]->value, tArray->tokens[index]);
      return node;
    }
  }
}

double evaluateAST(ASTNode *node, double x)
{
  if (strcmp(node->value, "tan") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    // printf("\nt:[%s]%lf\n", node->value, tan(l));
    return tan(l);
  }
  if (strcmp(node->value, "cos") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    return cos(l);
  }
  if (strcmp(node->value, "sin") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    return sin(l);
  }
  if (node->token->type == TOKEN_ARGUMENTS)
  {
    if (strcmp(node->value, "x") == 0)
    {
      return x;
    }
    else
    {
      if (isNumber(node->token->value))
      {
        return atof(node->token->value);
      }
    }
  }

  if (strcmp(node->value, "add") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    double l2 = evaluateAST(node->children[1], x);
    // printf("\n## %s:%lf ##\n", node->value, l);

    // printf("\nt:[%s]%lf, %lf\n", node->value, l,l2);
    return l + l2;
  }
  if (strcmp(node->value, "minus") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    double l2 = evaluateAST(node->children[1], x);
    // printf("\n## %s:%lf ##\n", node->value, l);

    // printf("\nt:[%s]%lf, %lf\n", node->value, l,l2);
    return l - l2;
  }
  if (strcmp(node->value, "multiply") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    double l2 = evaluateAST(node->children[1], x);
    // printf("\n## %s:%lf ##\n", node->value, l);

    // printf("\nt:[%s]%lf, %lf\n", node->value, l,l2);
    return l * l2;
  }
  if (strcmp(node->value, "divide") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    double l2 = evaluateAST(node->children[1], x);
    // printf("\n## %s:%lf ##\n", node->value, l);

    // printf("\nt:[%s]%lf, %lf\n", node->value, l,l2);
    return l / l2;
  }
  if (strcmp(node->value, "power") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    double l2 = evaluateAST(node->children[1], x);
    // printf("\n## %s:%lf ##\n", node->value, l);

    // printf("\nt:[%s]%lf, %lf\n", node->value, l,l2);
    return pow(l, l2);
  }
    if (strcmp(node->value, "greater") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    double l2 = evaluateAST(node->children[1], x);
    // printf("\n## %s:%lf ##\n", node->value, l);

    // printf("\nt:[%s]%lf, %lf\n", node->value, l,l2);
    return pow(l, l2);
  }
  if (strcmp(node->value, "exp") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    // printf("\n## %s:%lf ##\n", node->value, l);

    // printf("\nt:[%s]%lf, %lf\n", node->value, l,l2);
    return exp(l);
  }
  if (strcmp(node->value, "log10") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    // printf("\n## %s:%lf ##\n", node->value, l);

    // printf("\nt:[%s]%lf, %lf\n", node->value, l,l2);
    return log10(l);
  }
  if (strcmp(node->value, "log") == 0 || strcmp(node->value, "ln") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    // printf("\n## %s:%lf ##\n", node->value, l);

    // printf("\nt:[%s]%lf, %lf\n", node->value, l,l2);
    return log(l);
  }
  if (strcmp(node->value, "logBase") == 0)
  {
    double l = evaluateAST(node->children[0], x);
    double l2 = evaluateAST(node->children[1], x);

    // printf("\n## %s:%lf ##\n", node->value, l);

    // printf("\nt:[%s]%lf, %lf\n", node->value, l,l2);
    return logbase(l, l2);
  }
}

void display(ASTNode *ast)
{
  if (ast == NULL)
  {
    printf("end");
    return;
  }
  printf("%s", ast->value);
  printf("\n");
  printf("(");
  display(ast->children[0]);
  display(ast->children[1]);
  printf(")\n");
}

double f(char *code, double x)
{
  lexer_T *lexer = init_lexer(code);

  token_T *token = lexer_get_next_token(lexer);
  TokenArray *tokenArray = createTokenArray();
  addToTokenArray(tokenArray, token);
  while (token != NULL)
  {
    token = lexer_get_next_token(lexer);
    addToTokenArray(tokenArray, token);
  }
  int s = 0;
  for (int i = 0; i < tokenArray->size - 1; i++)
  {
    // printf("\n{[%s:%d]}\n", tokenArray->tokens[i]->value,
    //        tokenArray->tokens[i]->type);
  }

  // int token_count = sizeof(token_input) / sizeof(token_T);
  int token_count = tokenArray->size - 1;

  ASTNode *ast = parseExpression(tokenArray, token_count, 0);
  int t_index = 0;
  if (e_t->isError)
  {
    return 0;
  }

  double t = evaluateAST(ast, (22.0 / 7) / 3);
  return t;
}

void printInRed(const char *format, ...)
{
  va_list args;
  va_start(args, format);

  // ANSI escape code for red text: \x1B[31m
  // ANSI escape code to reset text color: \x1B[0m
  printf("\x1B[31m");
  vprintf(format, args);
  printf("\x1B[0m");

  va_end(args);
}

int main()
{
  e_t = init_error();
  // char *code = "add(sin(3), 4)";
  char t[100];
  printf("BCT 1.0.0 (tags/v1)"
         "Type a simple math equation\n");
  while (1)
  {
    printf("@root >>> ");
    int z = scanf("%[^\n]s", t);
    getchar();
    double result = f(t, (22.0 / 7) / 3);
    if (e_t->isError)
    {
      printInRed("[Error]:%s at token %d\n", e_t->type, e_t->index);
    }
    else
    {
      printf("%lf\n", result);
    }
    e_t->type = (void *)0;
    e_t->isError = false;
  }

  // printf("\nAns:%lf\n", f(code, (22.0 / 7) / 3));
  // x = (22.0 / 7) / 3;   =====> pi / 3

  return 0;
}
