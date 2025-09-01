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
  strncpy(cmd.cmdbuf, ctx->repl_buffer, MAX_CMD_LENGTH);
  char* token = strtok(ctx->repl_buffer, " \n");

  while (token) {
    cmd.args[cmd.argcount++] = token;
    token = strtok(NULL, " \n");
  }

  cmd.type = CMD_UNKNOWN;
  if (strcmp(cmd.args[0], "tokenize") == 0) cmd.type = CMD_TOKENIZE;
  if (strcmp(cmd.args[0], "parse") == 0)    cmd.type = CMD_PARSE;
  if (strcmp(cmd.args[0], "help") == 0)     cmd.type = CMD_HELP;
  if (strcmp(cmd.args[0], "exit") == 0)     cmd.type = CMD_EXIT;

  return cmd;
}

bool repl_prompt(repl_ctx* ctx) {
  for (int i = 0; i < 20; i++) printf("━");
  printf("\n");
  printf("[%s] >> ", repl_state_str(ctx->state));
  char* line = NULL;
  if (fgets(ctx->repl_buffer, MAX_CMD_LENGTH, stdin) != NULL) {
    ctx->repl_buffer[strcspn(ctx->repl_buffer, "\n")] = 0;
    return true;
  }
  else {
    if (feof(stdin)) {
      ctx->running = false;
      return false;
    }
  }
  return false;
}

static void repl_help() {
  printf("Help: repl\n");
  printf("tokenize : enter the tokenization repl state\n");
  printf("parse    : enter the parsing repl state\n");
  printf("exit     : exit the repl\n");
}

int repl_run() {
  repl_ctx ctx = {.state = STATE_DEFAULT, .running = true};
  while (ctx.running) {
    switch (ctx.state) {
      case STATE_DEFAULT: {
        if (!repl_prompt(&ctx)) continue;
        repl_command cmd = repl_parse_command(&ctx);
        if (cmd.type == CMD_TOKENIZE) ctx.state = STATE_TOKENIZE;
        if (cmd.type == CMD_PARSE)    ctx.state = STATE_PARSE;
        if (cmd.type == CMD_HELP)     repl_help();
        if (cmd.type == CMD_EXIT)     ctx.running = false;
        break;
      }
      case STATE_TOKENIZE: ctx.state = repl_command_tokenize(&ctx); break;
      case STATE_PARSE:    ctx.state = repl_command_parse(&ctx); break;
    }
  }
  return 0;
}
