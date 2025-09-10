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

  // for windows we need to rebind stdout, and stderr to the new file descriptors
  // so the windows CRT knows about them
#ifdef _WIN32
  if (out) {
      FILE *newOut = _fdopen(dup(1), "w");
      if (newOut) {
          *stdout = *newOut;           // copy FILE internals
          setvbuf(stdout, NULL, _IONBF, 0); 
          fclose(newOut);              // stdout now owns fd 1
      }
  }
  if (err) {
      FILE *newErr = _fdopen(dup(2), "w");
      if (newErr) {
          *stderr = *newErr;
          setvbuf(stderr, NULL, _IONBF, 0);
          fclose(newErr);
      }
  }
#endif
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
  
  fflush(NULL); // flush all open streams for extra safety
}
