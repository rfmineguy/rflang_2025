#include "tests.h"
#include "../../src/librfc/parser.h"
#include "../lib/test_util.h"
#include "../lib/munit_ext.h"
#define TEMP_FILE "file.tmp"

MunitResult parser_run_expr_single_number(const MunitParameter *param, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("4");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 2);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);


#undef actual
#define actual \
  "Expr.Disj.Conj.Rel.MathExpr.Term.Factor.Lit{int: 4}\n"

  // Test if the parsed output is correct
  munit_assert_int(r3.ok.ast_stack.size, ==, 2);
  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
    ast_node_print(result_.ok, 0);
    redirect_end(STDOUT_FILENO, handle);

    munit_assert_file_contents_equal(TEMP_FILE, actual);
  }, {
    munit_assert(false);
  });
  
  return MUNIT_OK;
}

MunitResult parser_run_expr_simple_addition(const MunitParameter *param, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("4 + 2");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 4);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);


#undef actual
#define actual \
  "Expr.Disj.Conj.Rel.MathExpr {\n"\
  "  Operator: +\n"\
  "  MathExpr.Term.Factor.Lit{int: 4}\n"\
  "  Term.Factor.Lit{int: 2}\n"\
  "}\n"

  // Test if the parsed output is correct
  munit_assert_int(r3.ok.ast_stack.size, ==, 2);
  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
    ast_node_print(result_.ok, 0);
    redirect_end(STDOUT_FILENO, handle);
    munit_assert_file_contents_equal(TEMP_FILE, actual);
  }, {
    munit_assert(false);
  });
  
  return MUNIT_OK;
}

MunitResult parser_run_expr_add_and_mul(const MunitParameter *param, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("4 + 2 * 9");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 6);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);


#undef actual
#define actual \
  "Expr.Disj.Conj.Rel.MathExpr {\n"\
  "  Operator: +\n"\
  "  MathExpr.Term.Factor.Lit{int: 4}\n"\
  "  Term {\n"\
  "    Operator: *\n"\
  "    Term.Factor.Lit{int: 2}\n"\
  "    Factor.Lit{int: 9}\n"\
  "  }\n"\
  "}\n"\

  // Test if the parsed output is correct
  munit_assert_int(r3.ok.ast_stack.size, ==, 2);
  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
    ast_node_print(result_.ok, 0);
    redirect_end(STDOUT_FILENO, handle);
    munit_assert_file_contents_equal(TEMP_FILE, actual);
  }, {
    munit_assert(false);
  });

  return MUNIT_OK;
}

MunitResult parser_run_expr_with_parens(const MunitParameter *param, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("(3 * (4 + 2)) * 9");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 12);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);

#undef actual
#define actual \
  "Expr.Disj.Conj.Rel.MathExpr.Term {\n"\
  "  Operator: *\n"\
  "  Term.Factor {\n"\
  "    Expr.Disj.Conj.Rel.MathExpr.Term {\n"\
  "      Operator: *\n"\
  "      Term.Factor.Lit{int: 3}\n"\
  "      Factor {\n"\
  "        Expr.Disj.Conj.Rel.MathExpr {\n"\
  "          Operator: +\n"\
  "          MathExpr.Term.Factor.Lit{int: 4}\n"\
  "          Term.Factor.Lit{int: 2}\n"\
  "        }\n"\
  "      }\n"\
  "    }\n"\
  "  }\n"\
  "  Factor.Lit{int: 9}\n"\
  "}\n"\

  // Test if the parsed output is correct
  munit_assert_int(r3.ok.ast_stack.size, ==, 2);
  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
    ast_node_print(result_.ok, 0);
    redirect_end(STDOUT_FILENO, handle);
    munit_assert_file_contents_equal(TEMP_FILE, actual);
  }, {
    munit_assert(false);
  });
  
  return MUNIT_OK;
}

MunitResult parser_run_expr_cmp_ops(const MunitParameter *param, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("((4 * 5) + 5) >= 1 / ((4 / 4) - 4)");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 22);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);

#undef actual
#define actual\
  "Expr.Disj.Conj.Rel {\n"\
  "  Operator: >=\n"\
  "  Rel.MathExpr.Term.Factor {\n"\
  "    Expr.Disj.Conj.Rel.MathExpr {\n"\
  "      Operator: +\n"\
  "      MathExpr.Term.Factor {\n"\
  "        Expr.Disj.Conj.Rel.MathExpr.Term {\n"\
  "          Operator: *\n"\
  "          Term.Factor.Lit{int: 4}\n"\
  "          Factor.Lit{int: 5}\n"\
  "        }\n"\
  "      }\n"\
  "      Term.Factor.Lit{int: 5}\n"\
  "    }\n"\
  "  }\n"\
  "  MathExpr.Term {\n"\
  "    Operator: /\n"\
  "    Term.Factor.Lit{int: 1}\n"\
  "    Factor {\n"\
  "      Expr.Disj.Conj.Rel.MathExpr {\n"\
  "        Operator: -\n"\
  "        MathExpr.Term.Factor {\n"\
  "          Expr.Disj.Conj.Rel.MathExpr.Term {\n"\
  "            Operator: /\n"\
  "            Term.Factor.Lit{int: 4}\n"\
  "            Factor.Lit{int: 4}\n"\
  "          }\n"\
  "        }\n"\
  "        Term.Factor.Lit{int: 4}\n"\
  "      }\n"\
  "    }\n"\
  "  }\n"\
  "}\n"\
  ""

  // Test if the parsed output is correct
  munit_assert_int(r3.ok.ast_stack.size, ==, 2);
  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
    ast_node_print(result_.ok, 0);
    redirect_end(STDOUT_FILENO, handle);
    munit_assert_file_contents_equal(TEMP_FILE, actual);
  }, {
    munit_assert(false);
  });

  return MUNIT_OK;
}

MunitResult parser_run_vardecl(const MunitParameter *param, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("a: int");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 4);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);

#undef actual
#define actual \
  "Var{\n"\
  "  id: a\n"\
  "  Type{id: int}\n"\
  "}\n"

  // Test if the parsed output is correct
  munit_assert_int(r3.ok.ast_stack.size, ==, 2);
  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantVar);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
    ast_node_print(result_.ok, 0);
    redirect_end(STDOUT_FILENO, handle);
    munit_assert_file_contents_equal(TEMP_FILE, actual);
  }, {
    munit_assert(false);
  });

  return MUNIT_OK;
}

MunitResult parser_run_type_ptr(const MunitParameter *param, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("*int");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 3);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);

#undef actual
#define actual \
  "Type[Ptr].Type{id: int}\n"

  munit_assert_int(r3.ok.ast_stack.size, ==, 2);
  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantType);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
    ast_node_print(result_.ok, 0);
    redirect_end(STDOUT_FILENO, handle);
    munit_assert_file_contents_equal(TEMP_FILE, actual);
  }, {
    munit_assert(false);
  });

  return MUNIT_OK;
}

MunitResult parser_run_type_ptr_ptr(const MunitParameter *param, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("**int");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 4);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);
#undef actual
#define actual \
  "Type[Ptr].Type[Ptr].Type{id: int}\n"

  munit_assert_int(r3.ok.ast_stack.size, ==, 2);
  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantType);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
    ast_node_print(result_.ok, 0);
    redirect_end(STDOUT_FILENO, handle);
    munit_assert_file_contents_equal(TEMP_FILE, actual);
  }, {
    munit_assert(false);
  });

  return MUNIT_OK;
}

MunitResult parser_run_type_array(const MunitParameter *param, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("[int]");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 4);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);
#undef actual
#define actual \
  "Type[Array]{\n"\
  "  Type{id: int}\n"\
  "}\n"

  munit_assert_int(r3.ok.ast_stack.size, ==, 2);
  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantType);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
    ast_node_print(result_.ok, 0);
    redirect_end(STDOUT_FILENO, handle);
    munit_assert_file_contents_equal(TEMP_FILE, actual);
  }, {
    munit_assert(false);
  });

  return MUNIT_OK;
}
MunitResult parser_run_type_array_of_ptr(const MunitParameter *param, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("[*int]");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 4);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);
#undef actual
#define actual \
  "Type[Array]{\n"\
  "  Type[Ptr].Type{id: int}\n"\
  "}\n"

  munit_assert_int(r3.ok.ast_stack.size, ==, 2);
  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantType);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
    ast_node_print(result_.ok, 0);
    redirect_end(STDOUT_FILENO, handle);
    munit_assert_file_contents_equal(TEMP_FILE, actual);
  }, {
    munit_assert(false);
  });

  return MUNIT_OK;
}
