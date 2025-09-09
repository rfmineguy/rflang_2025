#include "test_util.h"
#include <stdlib.h>
#ifdef _WIN32
#else
#include <sys/wait.h>
#endif

// code = -1   => failed to run system
process_result run_proc_cmd(const char* cmdstring) {
  process_result r = {0};
  int ret = system(cmdstring);
  if (ret == -1) {
    r.code = -1;
    return r;
  }
#ifdef _WIN32
  r.code = ret;
#else
  if (WIFEXITED(ret)) {
    r.code = WEXITSTATUS(ret);
  }
  else {
    r.code = -1;
  }
#endif
  return r;
}
