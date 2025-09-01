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
