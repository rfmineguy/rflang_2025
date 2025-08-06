#ifndef RESULT_H
#define RESULT_H
#include <stdbool.h>
#include <assert.h>

#define join(a, b) a##b

#define define_result(result_name, oktype, errtype)\
  typedef struct {\
    bool isok;\
    bool isfail;\
    oktype ok;\
    errtype err;\
  } join(result_, result_name);

#define match(result, result_name, okblock, errblock)\
{\
  join(result_, result_name) result_ = (result);\
  if (result_.isok) {\
    okblock\
  }\
  else {\
    errblock\
  }\
}

#define unwrap(result, result_name, okblock)\
{\
  join(result_, result_name) result_ = (result);\
  if (result_.isok) {\
    okblock;\
  }\
  else {\
    fprintf(stderr, "Unwrap of err type failed\n");\
    assert(0 && "");\
  }\
}

#define result_ok(result_name, okvalue)\
  (join(result_, result_name)) {.isok = true, .isfail = false, .ok = okvalue}

#define result_err(result_name, errvalue)\
  (join(result_, result_name)) {.isok = false, .isfail = true, .err = errvalue}

#endif
