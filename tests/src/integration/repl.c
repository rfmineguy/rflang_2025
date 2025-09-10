#include "tests.h"
#include "test_util.h"
#include "../../src/rfc/repl.h"
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
  line("Expr.Disj.Conj.Rel.MathExpr {")\
  line("  Operator: +")\
  line("  MathExpr.Term.Factor.Lit{id: a}")\
  line("  Term.Factor.Lit{int: 3}")\
  line("}")\
  line("Token(EOF_), ''")\

  // setup input file
  FILE* in = fopen("input.txt", "w+");
  FILE* out = fopen("output.txt", "w+");
  FILE* err = fopen("error.txt", "w+");
  fputs(input, in);
  rewind(in);

  repl_run_internal(in, out, err);

  fclose(in);
  fclose(out);
  fclose(err);

  munit_assert_file_contents_equal("output.txt", expected_output);

  return MUNIT_OK;
}
