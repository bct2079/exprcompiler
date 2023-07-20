#include "lexer.c"
#include <stdio.h>

typedef struct {
  token_T **tokens;
  int capacity;
  int size;
} TokenArray;

TokenArray *createTokenArray() {
  TokenArray *array = (TokenArray *)malloc(sizeof(TokenArray));
  array->tokens = NULL;
  array->capacity = 0;
  array->size = 0;
  return array;
}

void destroyTokenArray(TokenArray *array) {
  if (array == NULL)
    return;

  if (array->tokens != NULL) {
    for (int i = 0; i < array->size; i++) {
      free(array->tokens[i]->value);
      free(array->tokens[i]);
    }
    free(array->tokens);
  }

  free(array);
}

void addToTokenArray(TokenArray *array, token_T *token) {
  if (array->size == array->capacity) {
    // Double the capacity if the array is full
    int newCapacity = array->capacity * 2 + 1;
    token_T **newTokens =
        (token_T **)realloc(array->tokens, newCapacity * sizeof(token_T *));
    if (newTokens == NULL) {
      fprintf(stderr, "Memory reallocation failed\n");
      exit(1);
    }
    array->tokens = newTokens;
    array->capacity = newCapacity;
  }

  array->tokens[array->size] = token;
  array->size++;
}