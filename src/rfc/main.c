#include "librfc/tokenizer.h"
#include "librfc/parser.h"
#include <stdio.h>

int main() {
  tokenizer t;

  // Create the tokenizer
  match(tokenizer_create("samples/main.rf"), tokenizer_create, t = result_.ok;,{
    fprintf(stderr, "Error: %s\n", result_.err);
    return -1;
  })

  match(tokenizer_run(&t), tokenizer_run, {
    match(parser_run(&t), parser_run, {
      printf("Parser run successful\n");
    }, {
      printf("Parser run fail: %s\n", result_.err);
    });
  }, {
    fprintf(stderr, "Error: %s\n", result_.err);
    tokenizer_free(&t);
    return -2;
  })

  printf("%s\n", t.fileContents);
  tokenizer_free(&t);
}
