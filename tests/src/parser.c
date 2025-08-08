#include "tests.h"
#include "../../src/librfc/parser.h"

MunitResult parser_stack_check_test_1(const MunitParameter *param, void *context){
  stack_ast_node stack = stack_ast_node_create();
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = ID, .len = 1, .start = "x" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 2, .start = "42" }}});

  match(stack_check(&stack, check_seq({token(ID), token(EQ), token(EQ)})), stack_check, {
    munit_logf(1, "OK Count: %d", result_.ok.count);
  }, {
    munit_logf(1, "%s", result_.err);
    munit_assert(false);
  });

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult parser_stack_check_test_2(const MunitParameter *param, void *context){
  stack_ast_node stack = stack_ast_node_create();
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = ID, .len = 1, .start = "x" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_VAR,   .var   = {.placeholder = 3 }});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 2, .start = "42" }}});

  match(stack_check(&stack, check_seq({token(ID), ast(AST_VAR), token(EQ)})), stack_check, {
    munit_assert(true);
  }, {
    munit_assert(false);
  });

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult parser_stack_check_passed_limit_test(const MunitParameter *param, void *context){
  stack_ast_node stack = stack_ast_node_create();
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = ID, .len = 1, .start = "x" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_VAR,   .var   = {.placeholder = 3 }});

  // because the stack we are checking has only 2 items on it, but we are checking for 3
  //   we should expect this check to fail
  match(stack_check(&stack, check_seq({token(ID), ast(AST_VAR), token(EQ)})), stack_check, {
    munit_assert(false);
  }, {
    munit_assert(true);
  });

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult parser_stack_check_stack_larger_than_check(const MunitParameter *param, void *context){
  stack_ast_node stack = stack_ast_node_create();
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = LCBRK, .len = 1, .start = "{" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = ID, .len = 1, .start = "x" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_VAR,   .var =   {.placeholder = 4 }});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});

  match(stack_check(&stack, check_seq({token(ID), ast(AST_VAR), token(EQ)})), stack_check, {
    munit_logf(2, "ok: %d", result_.ok.count);
    munit_assert(true);
  }, {
    munit_logf(2, "err: %s", result_.err);
    munit_assert(false);
  });

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}
