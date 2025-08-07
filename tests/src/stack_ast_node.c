#include "tests.h"
#include "../../src/librfc/stack_ast_node.h"

MunitResult stack_ast_node_create_test(const MunitParameter *param, void *context) {
  stack_ast_node stack = stack_ast_node_create();
  munit_assert_int(stack.size, ==, 0);
  munit_assert_int(stack.capacity, ==, 10);
  munit_assert_not_null(stack.buffer);
  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_ast_node_push_test1(const MunitParameter *param, void *context) {
  stack_ast_node stack = stack_ast_node_create();
  stack_ast_node_push(&stack, (ast_node){.type = AST_INTLIT, .intlit = {.val = 2}});
  result_stack_ast_node_top r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.type, ==, AST_INTLIT);
  munit_assert_int(r.ok.intlit.val, ==, 2);

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_ast_node_push_pop_test(const MunitParameter *param, void *context) {
  stack_ast_node stack = stack_ast_node_create();
  // []
  stack_ast_node_push(&stack, (ast_node){.type = AST_INTLIT, .intlit = {.val = 2}});

  // [2]
  result_stack_ast_node_top r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.type, ==, AST_INTLIT);
  munit_assert_int(r.ok.intlit.val, ==, 2);

  // [2, 3]
  stack_ast_node_push(&stack, (ast_node){.type = AST_INTLIT, .intlit = {.val = 3}});
  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.intlit.val, ==, 3);

  // [2]
  stack_ast_node_pop(&stack);
  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.intlit.val, ==, 2);

  // [] after pop stack is empty here, so looking at top should fail
  stack_ast_node_pop(&stack);
  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isfail);

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_ast_node_top_offset_test(const MunitParameter *param, void *context) {
  stack_ast_node stack = stack_ast_node_create();
  stack_ast_node_push(&stack, (ast_node){.type = AST_INTLIT, .intlit = {.val = 2}});

  result_stack_ast_node_top r = stack_ast_node_top_offset(&stack, 0);
  munit_assert_true(r.isok);
  munit_assert_false(r.isfail);
  munit_assert_int(r.ok.type, ==, AST_INTLIT);
  munit_assert_int(r.ok.intlit.val, ==, 2);

  r = stack_ast_node_top_offset(&stack, 1);
  munit_assert_true(r.isfail);
  munit_assert_false(r.isok);

  stack_ast_node_push(&stack, (ast_node){.type = AST_INTLIT, .intlit = {.val = 2}});

  r = stack_ast_node_top_offset(&stack, 1);
  munit_assert_true(r.isok);
  munit_assert_false(r.isfail);
  munit_assert_int(r.ok.type, ==, AST_INTLIT);
  munit_assert_int(r.ok.intlit.val, ==, 2);

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_ast_node_top_test(const MunitParameter *param, void *context) {
  stack_ast_node stack = stack_ast_node_create();

  // We expect a call to top with an empty stack to fail
  result_stack_ast_node_top r = stack_ast_node_top(&stack);
  munit_assert_false(r.isok);
  munit_assert_true(r.isfail);

  // We expect a call to top with a single item to succeed with that item
  stack_ast_node_push(&stack, (ast_node){.type = AST_INTLIT, .intlit = {.val = 2}});
  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_false(r.isfail);
  munit_assert_int(r.ok.type, ==, AST_INTLIT);
  munit_assert_int(r.ok.intlit.val, ==, 2);

  // We expect a call to top with a single item to succeed with that item
  stack_ast_node_push(&stack, (ast_node){.type = AST_INTLIT, .intlit = {.val = 2}});
  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_false(r.isfail);
  munit_assert_int(r.ok.type, ==, AST_INTLIT);
  munit_assert_int(r.ok.intlit.val, ==, 2);

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}
