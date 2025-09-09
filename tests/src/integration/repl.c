#include "tests.h"
#include "test_util.h"
#include <unistd.h>
#include <stdio.h>
#ifndef RFC_PATH
#define RFC_PATH ""
#endif

MunitResult rfc_repl_test_blank_input(const MunitParameter* params, void* fixture) {
  munit_assert_string_not_equal(RFC_PATH, ""); //TODO: Replace with an assertion that the path exists

  // setup mock input
#define input\
  "parse\n"\
  "a + 3\n"\

#define expected_output\
  "Expr.Disj.Conj.Rel.MathExpr {\n"\
  "  Operator: +\n"\
  "  MathExpr.Term.Factor.Lit{id: a}\n"\
  "  Term.Factor.Lit{int: 3}\n"\
  "}\n"\
  "Token(EOF_), ''\n"\

  // setup input file
  FILE* f = fopen("input.txt", "w");
  fputs(input, f);
  fclose(f);

  process_result r = run_proc_cmd(RFC_PATH " --repl" " < input.txt > output.txt 2> prompts.txt");
  if (r.code == -1) return MUNIT_ERROR;

  munit_assert_file_contents_equal("output.txt", expected_output);

  return MUNIT_OK;
}
