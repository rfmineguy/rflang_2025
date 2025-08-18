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

#ifdef _WIN32
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define STDOUT_FILENO _fileno(stdout)
#define flush_fd(fd) _commit(fd)
#define OPEN_FLAGS O_WRONLY | O_CREAT | O_TRUNC | O_BINARY
#else
#include <unistd.h>
#define flush_fd(fd) fsync(fd)
#define OPEN_FLAGS O_WRONLY | O_CREAT | O_TRUNC
#endif

// Begin redirect macro
#define redirect_begin(fd_from, filename_to, handle)         \
  int handle = dup(fd_from);                                 \
  do {                                                       \
    fflush(stdout);                                          \
    flush_fd(fd_from);                                       \
    int __redirect_fd = open(filename_to, OPEN_FLAGS, 0644); \
    dup2(__redirect_fd, fd_from);                            \
    close(__redirect_fd);                                    \
  } while (0)

// End redirect macro
#define redirect_end(fd_from, handle)    \
  do {                                   \
    fflush(stdout);                      \
    flush_fd(fd_from);                   \
    dup2(handle, fd_from);      \
    close(fd_from);              \
  } while(0)

#endif
