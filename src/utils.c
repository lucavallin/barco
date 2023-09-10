#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// author: ChatGPT.
char *join_strings(char *strings[], int start, int end) {
  char *separator = " ";
  if (strings == NULL || end < start) {
    return NULL;
  }
  size_t total_length = 0;
  for (int i = start; i <= end; i++) {
    total_length += strlen(strings[i]);
  }
  total_length += (end - start - 1) * strlen(separator) + 1;

  char *result = (char *)malloc(total_length);
  if (result == NULL) {
    return NULL;
  }

  result[0] = '\0';

  for (int i = start; i <= end; i++) {
    strncat(result, strings[i], strlen(strings[i]));
    if (i != end) {
      strncat(result, separator, strlen(separator));
    }
  }

  return result;
}
