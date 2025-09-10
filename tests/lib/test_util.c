#include "test_util.h"

void redirect_begin_(FILE* in, FILE* out, FILE* err, stdio_ctx* save) {
  save->saved_in = dup(0);
  save->saved_out = dup(1);
  save->saved_err = dup(2);

  fflush(stdout);
  fflush(stderr);

  if (in)  dup2(fileno(in), 0);
  if (out) dup2(fileno(out), 1);
  if (err) dup2(fileno(err), 2);
}
void redirect_end_(stdio_ctx* save) {
  fflush(stdout);
  fflush(stderr);

  dup2(save->saved_in, 0);
  dup2(save->saved_out, 1);
  dup2(save->saved_err, 2);

  close(save->saved_in);
  close(save->saved_out);
  close(save->saved_err);
}
