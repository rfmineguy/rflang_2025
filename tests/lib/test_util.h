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
#include <fcntl.h>
#define open_file _open
#define close_file _close
#define fdopen_file _fdopen
#define O_BINARY_FLAG _O_BINARY
#else
#include <fcntl.h>
#include <unistd.h>
#define open_file open
#define close_file close
#define fdopen_file fdopen
#define O_BINARY_FLAG 0
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
{ \
    stdio_ctx ctx = {0}; \
    FILE* stdin_ = NULL; \
    FILE* stdout_ = NULL; \
    FILE* stderr_ = NULL; \
    int fd_in = -1, fd_out = -1, fd_err = -1; \
    if (stdin_fn_) { \
        fd_in = open_file(stdin_fn_, O_RDONLY | O_BINARY_FLAG); \
        if (fd_in != -1) stdin_ = fdopen_file(fd_in, "rb"); \
        if (!stdin_ && fd_in != -1) close_file(fd_in); \
    } \
    if (stdout_fn_) { \
        fd_out = open_file(stdout_fn_, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY_FLAG, 0644); \
        if (fd_out != -1) stdout_ = fdopen_file(fd_out, "wb"); \
        if (!stdout_ && fd_out != -1) close_file(fd_out); \
    } \
    if (stderr_fn_) { \
        fd_err = open_file(stderr_fn_, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY_FLAG, 0644); \
        if (fd_err != -1) stderr_ = fdopen_file(fd_err, "wb"); \
        if (!stderr_ && fd_err != -1) close_file(fd_err); \
    } \
    redirect_begin_(stdin_, stdout_, stderr_, &ctx);

#define redirect_to_filename_end()\
  redirect_end_(&ctx); \
  if (stdin_) fclose(stdin_); \
  if (stdout_) fclose(stdout_); \
  if (stderr_) fclose(stderr_); \
}
#endif
