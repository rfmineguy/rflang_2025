#include "tokenizer.h"
#include <stdio.h>

int main() {
  tokenizer t;

  // Create the tokenizer
  match(tokenizer_create("samples/main.rf"), tokenizer_create, t = result_.ok;,{
    fprintf(stderr, "Error: %s\n", result_.err);
    return -1;
  })

  match(tokenizer_run(&t), tokenizer_run, {}, {
    fprintf(stderr, "Error: %s\n", result_.err);
    return -2;
  })

  printf("%s\n", t.fileContents);
  tokenizer_free(&t);
}
