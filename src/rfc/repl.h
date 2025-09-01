#ifndef REPL_H
#define REPL_H
#include <stdbool.h>
#define MAX_CMD_LENGTH 255

typedef enum {
  STATE_DEFAULT,
  STATE_TOKENIZE,
  STATE_PARSE,
} repl_state;

typedef enum {
  CMD_TOKENIZE,
  CMD_PARSE,
  CMD_HELP,
  CMD_EXIT,
  CMD_UNKNOWN,
} repl_command_type;

typedef struct {
  repl_command_type type;
  const char* args[20];
  char cmdbuf[MAX_CMD_LENGTH];
  int argcount;
} repl_command;

typedef struct {
  repl_state state;
  char repl_buffer[MAX_CMD_LENGTH];
  bool running;
} repl_ctx;

bool repl_prompt(repl_ctx* ctx);
repl_command repl_parse_command(repl_ctx* ctx);
int repl_run();

#endif
