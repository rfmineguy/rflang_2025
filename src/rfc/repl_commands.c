#include "repl_commands.h"
#include <stdio.h>

static void repl_help_tokenize() {
  printf("Help: tokenize\n");
  printf("   Type text into the prompt and it will be output as tokens\n");
}
static void repl_help_parse() {
  printf("Help: parse\n");
  printf("   Type text into the prompt and it will be output as an ast\n");
}
