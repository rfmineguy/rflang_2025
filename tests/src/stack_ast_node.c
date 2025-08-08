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
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});
  result_stack_ast_node_top r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.type, ==, AST_TOKEN);
  munit_assert_int(r.ok.token.t.type, ==, EQ);
  munit_assert_int(r.ok.token.t.len, ==, 1);
  munit_assert_string_equal(r.ok.token.t.start, "=");

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_ast_node_push_pop_test(const MunitParameter *param, void *context) {
  stack_ast_node stack = stack_ast_node_create();
  // []
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});

  // [2]
  result_stack_ast_node_top r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.type, ==, AST_TOKEN);
  munit_assert_int(r.ok.token.t.type, ==, EQ);
  munit_assert_int(r.ok.token.t.len, ==, 1);
  munit_assert_string_equal(r.ok.token.t.start, "=");

  // [2, 3]
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = LCBRK, .len = 1, .start = "{" }}});
  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.type, ==, AST_TOKEN);
  munit_assert_int(r.ok.token.t.type, ==, LCBRK);
  munit_assert_int(r.ok.token.t.len, ==, 1);
  munit_assert_string_equal(r.ok.token.t.start, "{");

  // [2]
  result_stack_ast_node_pop r2 = stack_ast_node_pop(&stack);
  munit_assert_true(r2.isok);

  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.type, ==, AST_TOKEN);
  munit_assert_int(r.ok.token.t.type, ==, EQ);
  munit_assert_int(r.ok.token.t.len, ==, 1);
  munit_assert_string_equal(r.ok.token.t.start, "=");

  // [] after pop stack is empty here, so looking at top should fail
  stack_ast_node_pop(&stack);
  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isfail);

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_ast_node_push_pop_n_test(const MunitParameter *param, void *context) {
  stack_ast_node stack = stack_ast_node_create();
  // Test popping everything off
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});

  munit_assert_int(stack.size, ==, 3);
  result_stack_ast_node_pop_n r = stack_ast_node_pop_n(&stack, 3);
  munit_assert_true(r.isok);
  munit_assert_int(stack.size, ==, 0);

  // Test popping less than the size off
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});

  munit_assert_int(stack.size, ==, 3);
  r = stack_ast_node_pop_n(&stack, 2);
  munit_assert_true(r.isok);
  munit_assert_int(stack.size, ==, 1);

  stack_ast_node_pop(&stack);

  // Test popping more than the size off
  //  Expected: the stack does not change
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});

  munit_assert_int(stack.size, ==, 3);
  r = stack_ast_node_pop_n(&stack, 4);
  munit_assert_true(r.isfail);
  munit_assert_int(stack.size, ==, 3);

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_ast_node_top_offset_test(const MunitParameter *param, void *context) {
  stack_ast_node stack = stack_ast_node_create();
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});
  munit_assert_int(stack.size, ==, 1);

  result_stack_ast_node_top r = stack_ast_node_top_offset(&stack, 0);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.type, ==, AST_TOKEN);
  munit_assert_int(r.ok.token.t.type, ==, EQ);
  munit_assert_int(r.ok.token.t.len, ==, 1);
  munit_assert_string_equal(r.ok.token.t.start, "=");

  r = stack_ast_node_top_offset(&stack, 1);
  munit_assert_true(r.isfail);
  munit_assert_false(r.isok);

  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = LCBRK, .len = 1, .start = "{" }}});
  munit_assert_int(stack.size, ==, 2);

  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.type, ==, AST_TOKEN);
  munit_assert_int(r.ok.token.t.type, ==, LCBRK);
  munit_assert_int(r.ok.token.t.len, ==, 1);
  munit_assert_string_equal(r.ok.token.t.start, "{");

  munit_assert_int(stack.size, ==, 2);

  result_stack_ast_node_pop_n r2 = stack_ast_node_pop_n(&stack, 2);
  munit_assert_true(r2.isok);
  munit_assert_int(stack.size, ==, 0);

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
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = EQ, .len = 1, .start = "=" }}});
  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_false(r.isfail);
  munit_assert_int(r.ok.type, ==, AST_TOKEN);
  munit_assert_int(r.ok.token.t.type, ==, EQ);
  munit_assert_int(r.ok.token.t.len, ==, 1);
  munit_assert_string_equal(r.ok.token.t.start, "=");

  // We expect a call to top with a single item to succeed with that item
  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = COLON, .len = 1, .start = ":" }}});
  r = stack_ast_node_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_false(r.isfail);
  munit_assert_int(r.ok.type, ==, AST_TOKEN);
  munit_assert_int(r.ok.token.t.type, ==, COLON);
  munit_assert_int(r.ok.token.t.len, ==, 1);
  munit_assert_string_equal(r.ok.token.t.start, ":");

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_ast_node_empty_test(const MunitParameter *param, void *context) {
  stack_ast_node stack = stack_ast_node_create();
  munit_assert_true(stack_ast_node_empty(&stack));

  stack_ast_node_push(&stack, (ast_node){.type = AST_TOKEN, .token = {.t = { .type = COLON, .len = 1, .start = ":" }}});
  munit_assert_false(stack_ast_node_empty(&stack));

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}
