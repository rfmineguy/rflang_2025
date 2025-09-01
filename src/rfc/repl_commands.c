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

repl_state repl_command_tokenize(repl_ctx* ctx) {
  if (!repl_prompt(ctx)) return STATE_TOKENIZE;
  repl_command cmd = repl_parse_command(ctx);
  if (cmd.type == CMD_EXIT) return STATE_DEFAULT;
  if (cmd.type == CMD_HELP) repl_help_tokenize();
  else {

  }
  return STATE_TOKENIZE;
}
repl_state repl_command_parse(repl_ctx* ctx) {
  if (!repl_prompt(ctx)) return STATE_PARSE;
  repl_command cmd = repl_parse_command(ctx);
  if (cmd.type == CMD_EXIT) return STATE_DEFAULT;
  if (cmd.type == CMD_HELP) repl_help_parse();
  else {

  }
  return STATE_PARSE;
}
