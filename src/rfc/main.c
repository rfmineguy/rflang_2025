#include "librfc/tokenizer.h"
#include "librfc/parser.h"
#include "spall_util.h"
#include "cmdline.h"
#include "repl.h"
#include <stdio.h>

int main(int argc, char** argv) {
  SPALL_INIT
  tokenizer t;

  struct gengetopt_args_info args_info;
  if (cmdline_parser(argc, argv, &args_info) != 0) return 1;

  if (args_info.repl_given) {
    return repl_run();
  }


  // Create the tokenizer
  match(tokenizer_create_file("samples/main.rf"), tokenizer_create, t = result_.ok;,{
    fprintf(stderr, "Error: %s\n", result_.err);
    return -1;
  })

  match(tokenizer_run(&t), tokenizer_run, {
    // Print out the token list
    for (int i = 0; i < t.tokens.size; i++)
      token_print(t.tokens.buffer[i]);

    // Pass the tokenizer off to the parser
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

  tokenizer_free(&t);
  SPALL_QUIT
}
