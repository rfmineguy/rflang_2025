#ifndef REPL_COMMANDS_H
#define REPL_COMMANDS_H
#include "repl.h"

repl_state repl_command_tokenize(repl_ctx* ctx);
repl_state repl_command_parse(repl_ctx* ctx);

#endif
