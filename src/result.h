#ifndef RESULT_H
#define RESULT_H

#define join(a, b) a##b

#define define_result(result_name, oktype, errtype)\
  typedef struct {\
    int isok;\
    int isfail;\
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

#define result_ok(result_name, okvalue)\
  (join(result_, result_name)) {.isok = 1, .isfail = 0, .ok = okvalue}

#define result_err(result_name, errvalue)\
  (join(result_, result_name)) {.isok = 0, .isfail = 1, .err = errvalue}

#endif
