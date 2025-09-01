#include "repl.h"
#include "repl_commands.h"
#include <stdio.h>
#include <string.h>

const char* repl_state_str(repl_state state) {
  switch (state) {
    case STATE_DEFAULT:  return "Default";
    case STATE_PARSE:    return "Parse";
    case STATE_TOKENIZE: return "Tokenize";
  }
}

repl_command repl_parse_command(repl_ctx* ctx) {
  repl_command cmd = {};
  char* token = ctx->repl_buffer;
  char* end = ctx->repl_buffer;

  while (token) {
    strsep(&end, " \n");
    cmd.args[cmd.argcount++] = token;
    token = end;
  }

  cmd.type = CMD_UNKNOWN;
  if (strcmp(cmd.args[0], "tokenize") == 0) cmd.type = CMD_TOKENIZE;
  if (strcmp(cmd.args[0], "parse") == 0)    cmd.type = CMD_PARSE;
  if (strcmp(cmd.args[0], "help") == 0)     cmd.type = CMD_HELP;
  if (strcmp(cmd.args[0], "exit") == 0)     cmd.type = CMD_EXIT;

  return cmd;
}

bool repl_prompt(repl_ctx* ctx) {
  printf("[%s] >> ", repl_state_str(ctx->state));
  char* line = NULL;
  if (fgets(ctx->repl_buffer, 255, stdin) != NULL) {
    ctx->repl_buffer[strcspn(ctx->repl_buffer, "\n")] = 0;
    return true;
  }
  return false;
}
