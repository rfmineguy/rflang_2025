#include "repl_commands.h"
#include "librfc/tokenizer.h"
#include "librfc/parser.h"
#include "librfc/result.h"
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
    tokenizer t;
    match(tokenizer_create_cstr(cmd.cmdbuf), tokenizer_create, { t = result_.ok; }, { return STATE_DEFAULT; });
    match(tokenizer_run(&t), tokenizer_run, {
      for (int i = 0; i < t.tokens.size; i++) {
        token_print(t.tokens.buffer[i]);
      }
    }, {
      printf("Error: %s\n", result_.err);
      tokenizer_free(&t);
      return STATE_DEFAULT;
    });
  }
  return STATE_TOKENIZE;
}
repl_state repl_command_parse(repl_ctx* ctx) {
  if (!repl_prompt(ctx)) return STATE_PARSE;
  repl_command cmd = repl_parse_command(ctx);
  if (cmd.type == CMD_EXIT) return STATE_DEFAULT;
  if (cmd.type == CMD_HELP) repl_help_parse();
  else {
    tokenizer t;
    match(tokenizer_create_cstr(cmd.cmdbuf), tokenizer_create, { t = result_.ok; }, { return STATE_DEFAULT; });
    match(tokenizer_run(&t), tokenizer_run, {
      match(parser_run(&t), parser_run, {
        stack_ast_node_print(&result_.ok.ast_stack);
        parser_free(&result_.ok);
        tokenizer_free(&t);
      }, {
        printf("Error: %s\n", result_.err);
      })
    }, {
      printf("Error: %s\n", result_.err);
      tokenizer_free(&t);
    });
  }
  return STATE_PARSE;
}
