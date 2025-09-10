/*  This is a util header for the test suite
 *  Features:
 *    1. redirect
 *
 *
 *  'redirect'
 *    - this feature is used to redirect output from one file descriptor to another
 *    + redirect_begin(fd_from, filename_to)
 *        #fd_from     : where are we redirecting from?
 *        #filename_to : what filename do we redirect to?
 *    + redirect_end(fd_from)
 *        #fd_from     : where did we redirect from?
 *    Example:
 *    ```c
 *        // redirect output temporarily to file named "file"
 *        redirect_begin(STDOUT_FILENO, "file");
 *        printf("hi\n");
 *        redirect_end(STDOUT_FILENO);
 *    ```
 */
#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <stdio.h>
#include <fcntl.h>

typedef struct {
  int code;
} process_result;

#ifdef _WIN32
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define STDOUT_FILENO _fileno(stdout)
#define flush_fd(fd) _commit(fd)
#define OPEN_FLAGS O_WRONLY | O_CREAT | O_TRUNC | O_BINARY
#define line(out) out "\r\n"
#else
#include <unistd.h>
#define flush_fd(fd) fsync(fd)
#define OPEN_FLAGS O_WRONLY | O_CREAT | O_TRUNC
#define line(out) out "\n"
#endif

typedef struct {
  int saved_in, saved_out, saved_err;
} stdio_ctx;

void redirect_begin_(FILE* in, FILE* out, FILE* err, stdio_ctx* save);
void redirect_end_(stdio_ctx* save);

#define redirect_to_file_begin(stdin_, stdout_, stderr_)\
{\
  stdio_ctx ctx = {0};\
  redirect_begin_(stdin_, stdout_, stderr_, &ctx);\

#define redirect_to_file_end()\
  redirect_end_(&ctx);\
}

#define redirect_to_filename_begin(stdin_fn_, stdout_fn_, stderr_fn_)\
{\
  stdio_ctx ctx = {0};\
  FILE* stdin_  = stdin_fn_ ? fopen(stdin_fn_, "r") : NULL;\
  FILE* stdout_ = stdout_fn_ ? fopen(stdout_fn_, "w") : NULL;\
  FILE* stderr_ = stderr_fn_ ? fopen(stderr_fn_, "w") : NULL;\
  redirect_begin_(stdin_, stdout_, stderr_, &ctx);\

#define redirect_to_filename_end()\
  redirect_end_(&ctx);\
}
  // fclose(stdin_);\
  // fclose(stdout_);\
  // fclose(stderr_);\


#endif
