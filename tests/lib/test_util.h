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

#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define STDOUT_FILENO _fileno(stdout)
#else
#include <unistd.h>
#endif

#define redirect_begin(fd_from, filename_to)\
  int orig = dup(fd_from);\
  const char* fn = filename_to;\
  int fd = open(filename_to, O_WRONLY | O_CREAT | O_TRUNC, 0644);\
  dup2(fd, fd_from);\
  close(fd);\

#define redirect_end(fd_from)\
  dup2(orig, fd_from);\
  close(orig);\


#endif
