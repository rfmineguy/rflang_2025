#include "spall.h"
#include <time.h>

#ifdef ENABLE_SPALL
#define MAX_CACHED_SYMBOLS 1000
#define SPALL_BUFFER_SIZE 10 * 1024 * 1024
#include "instrument.h"
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

extern SpallBuffer spall_buffer;
extern SpallProfile spall_ctx;
#define SPALL_INIT \
  init_profile("rfc.spall");\
  init_thread(0, SPALL_BUFFER_SIZE, MAX_CACHED_SYMBOLS, "main");

#define SPALL_QUIT \
  exit_thread();\
  exit_profile();

void init_profile(char *filename);
void exit_profile(void);
void init_thread(uint32_t tid, size_t buffer_size, int64_t symbol_cache_size, char *thread_name);
void exit_thread(void);
uint64_t get_time_in_nanos(void);
#else
#define SPALL_INIT {}
#define SPALL_QUIT {}
#endif
