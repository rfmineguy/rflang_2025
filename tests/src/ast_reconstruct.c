#include "tests.h"
#include "test_util.h"
#include "tokenizer.h"
#include "parser.h"
#include "ast_reconstruct.h"
#define TEMP_FILE "file.tmp"

MunitResult ast_reconstruct_vardecl(const MunitParameter* params, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("a: int");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 4);

  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);
  munit_assert_int(r3.ok.ast_stack.size, ==, 2);

  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantVar);

    redirect_begin(STDOUT_FILENO, TEMP_FILE, x);
    ast_var_reconstruct(result_.ok.VariantVar);
    redirect_end(STDOUT_FILENO, x);

    munit_assert_file_contents_equal(TEMP_FILE, "a: int");
  }, {
    munit_assert(false);
  });

  tokenizer_free(&r.ok);
  return MUNIT_OK;
}

MunitResult ast_reconstruct_expr_simple_addition(const MunitParameter* params, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("4 + 2");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 4);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);

  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

#undef TEMP_FILE
#define TEMP_FILE "other.tmp"
    redirect_begin(STDOUT_FILENO, TEMP_FILE, x);
    ast_expr_reconstruct(result_.ok.VariantExpr);
    redirect_end(STDOUT_FILENO, x);

    munit_assert_file_contents_equal(TEMP_FILE, "4+2");
  }, {
    munit_assert(false);
  });
  return MUNIT_OK;
}

MunitResult ast_reconstruct_expr_addition_with_mul(const MunitParameter* params, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("4 + 2 * 9");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 6);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);

  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

#undef TEMP_FILE
#define TEMP_FILE "other.tmp"
    redirect_begin(STDOUT_FILENO, TEMP_FILE, x);
    ast_expr_reconstruct(result_.ok.VariantExpr);
    redirect_end(STDOUT_FILENO, x);

    munit_assert_file_contents_equal(TEMP_FILE, "4+2*9");
  }, {
    munit_assert(false);
  });
  return MUNIT_OK;
}

MunitResult ast_reconstruct_expr_addition_with_paren(const MunitParameter* params, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("(4 + 2) * 9");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 8);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);

  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

#undef TEMP_FILE
#define TEMP_FILE "other.tmp"
    redirect_begin(STDOUT_FILENO, TEMP_FILE, x);
    ast_expr_reconstruct(result_.ok.VariantExpr);
    redirect_end(STDOUT_FILENO, x);

    munit_assert_file_contents_equal(TEMP_FILE, "(4+2)*9");
  }, {
    munit_assert(false);
  });
  return MUNIT_OK;
}

MunitResult ast_reconstruct_expr_mul_div(const MunitParameter* params, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("(4 * 5) / 4");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 8);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);

  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

#undef TEMP_FILE
#define TEMP_FILE "other.tmp"
    redirect_begin(STDOUT_FILENO, TEMP_FILE, x);
    ast_expr_reconstruct(result_.ok.VariantExpr);
    redirect_end(STDOUT_FILENO, x);

    munit_assert_file_contents_equal(TEMP_FILE, "(4*5)/4");
  }, {
    munit_assert(false);
  });
  return MUNIT_OK;
}

MunitResult ast_reconstruct_expr_mul_div_2(const MunitParameter* params, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("4 * 5 / 4");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 6);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);

  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

#undef TEMP_FILE
#define TEMP_FILE "other.tmp"
    redirect_begin(STDOUT_FILENO, TEMP_FILE, x);
    ast_expr_reconstruct(result_.ok.VariantExpr);
    redirect_end(STDOUT_FILENO, x);

    munit_assert_file_contents_equal(TEMP_FILE, "4*5/4");
  }, {
    munit_assert(false);
  });
  return MUNIT_OK;
}

MunitResult ast_reconstruct_expr_mul_div_3(const MunitParameter* params, void *context) {
  result_tokenizer_create r = tokenizer_create_cstr("4 * (5 / 4)");
  munit_assert_true(r.isok);
  result_tokenizer_run r2 = tokenizer_run(&r.ok);
  munit_assert_true(r2.isok);
  munit_assert_int(r.ok.tokens.size, ==, 8);

  // Test parser run
  result_parser_run r3 = parser_run(&r.ok);
  munit_assert_true(r3.isok);

  match(stack_ast_node_top_offset(&r3.ok.ast_stack, 1), stack_ast_node_top, {
    munit_assert_int(result_.ok.type, ==, variant_ast_node_type_VariantExpr);

#undef TEMP_FILE
#define TEMP_FILE "other.tmp"
    redirect_begin(STDOUT_FILENO, TEMP_FILE, x);
    ast_expr_reconstruct(result_.ok.VariantExpr);
    redirect_end(STDOUT_FILENO, x);

    munit_assert_file_contents_equal(TEMP_FILE, "4*(5/4)");
  }, {
    munit_assert(false);
  });
  return MUNIT_OK;
}
