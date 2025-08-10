#include "tests.h"
#include "generated/stack_test_type.h"

MunitResult stack_test_type_create_test(const MunitParameter *param, void *context) {
  stack_test_type stack = stack_test_type_create();
  munit_assert_int(stack.size, ==, 0);
  munit_assert_int(stack.capacity, ==, 10);
  munit_assert_not_null(stack.buffer);
  stack_test_type_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_test_type_push_test(const MunitParameter *param, void *context) {
  stack_test_type stack = stack_test_type_create();
  stack_test_type_push(&stack, (test_type){.x = 3, .s = "hello"});
  result_stack_test_type_top r = stack_test_type_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.x, ==, 3);
  munit_assert_string_equal(r.ok.s, "hello");

  stack_test_type_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_test_type_push_pop_test(const MunitParameter *param, void *context) {
  stack_test_type stack = stack_test_type_create();
  // []
  stack_test_type_push(&stack, (test_type){.x = 3, .s = "hello"});

  // [2]
  result_stack_test_type_top r = stack_test_type_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.x, ==, 3);
  munit_assert_string_equal(r.ok.s, "hello");

  // [2, 3]
  stack_test_type_push(&stack, (test_type){.x = 3, .s = "hello"});
  r = stack_test_type_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.x, ==, 3);
  munit_assert_string_equal(r.ok.s, "hello");

  // [2]
  result_stack_test_type_pop r2 = stack_test_type_pop(&stack);
  munit_assert_true(r2.isok);

  r = stack_test_type_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.x, ==, 3);
  munit_assert_string_equal(r.ok.s, "hello");

  // [] after pop stack is empty here, so looking at top should fail
  stack_test_type_pop(&stack);
  r = stack_test_type_top(&stack);
  munit_assert_true(r.isfail);

  stack_test_type_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_test_type_push_pop_n_test(const MunitParameter *param, void *context) {
  stack_test_type stack = stack_test_type_create();
  // Test popping everything off
  stack_test_type_push(&stack, (test_type){.x = 3, .s = "goodbye"});
  stack_test_type_push(&stack, (test_type){.x = 4, .s = "hello"});
  stack_test_type_push(&stack, (test_type){.x = 5, .s = "booger"});

  munit_assert_int(stack.size, ==, 3);
  result_stack_test_type_pop_n r = stack_test_type_pop_n(&stack, 3);
  munit_assert_true(r.isok);
  munit_assert_int(stack.size, ==, 0);

  // Test popping less than the size off
  stack_test_type_push(&stack, (test_type){.x = 4, .s = "remaining"});
  stack_test_type_push(&stack, (test_type){.x = 4, .s = "hello"});
  stack_test_type_push(&stack, (test_type){.x = 4, .s = "hello"});

  munit_assert_int(stack.size, ==, 3);
  r = stack_test_type_pop_n(&stack, 2);
  munit_assert_true(r.isok);
  munit_assert_int(stack.size, ==, 1);

  stack_test_type_pop(&stack);

  // Test popping more than the size off
  //  Expected: the stack does not change
  stack_test_type_push(&stack, (test_type){.x = 4, .s = "georgie"});
  stack_test_type_push(&stack, (test_type){.x = 4, .s = "afjjd"});
  stack_test_type_push(&stack, (test_type){.x = 4, .s = "hello"});

  munit_assert_int(stack.size, ==, 3);
  r = stack_test_type_pop_n(&stack, 4);
  munit_assert_true(r.isfail);
  munit_assert_int(stack.size, ==, 3);

  stack_test_type_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_test_type_top_offset_test(const MunitParameter *param, void *context) {
  stack_test_type stack = stack_test_type_create();
  stack_test_type_push(&stack, (test_type){.x = 4, .s = "hello"});
  munit_assert_int(stack.size, ==, 1);

  result_stack_test_type_top r = stack_test_type_top_offset(&stack, 0);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.x, ==, 4);
  munit_assert_string_equal(r.ok.s, "hello");

  r = stack_test_type_top_offset(&stack, 1);
  munit_assert_true(r.isfail);
  munit_assert_false(r.isok);

  stack_test_type_push(&stack, (test_type){.x = 4, .s = "hello"});
  munit_assert_int(stack.size, ==, 2);

  r = stack_test_type_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_true(r.isok);
  munit_assert_int(r.ok.x, ==, 4);
  munit_assert_string_equal(r.ok.s, "hello");

  munit_assert_int(stack.size, ==, 2);

  result_stack_test_type_pop_n r2 = stack_test_type_pop_n(&stack, 2);
  munit_assert_true(r2.isok);
  munit_assert_int(stack.size, ==, 0);

  stack_test_type_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_test_type_top_test(const MunitParameter *param, void *context) {
  stack_test_type stack = stack_test_type_create();

  // We expect a call to top with an empty stack to fail
  result_stack_test_type_top r = stack_test_type_top(&stack);
  munit_assert_false(r.isok);
  munit_assert_true(r.isfail);

  // We expect a call to top with a single item to succeed with that item
  stack_test_type_push(&stack, (test_type){.x = 4, .s = "hello"});
  r = stack_test_type_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_false(r.isfail);
  munit_assert_int(r.ok.x, ==, 4);
  munit_assert_string_equal(r.ok.s, "hello");

  // We expect a call to top with a single item to succeed with that item
  stack_test_type_push(&stack, (test_type){.x = 5, .s = "aello"});
  r = stack_test_type_top(&stack);
  munit_assert_true(r.isok);
  munit_assert_false(r.isfail);
  munit_assert_int(r.ok.x, ==, 5);
  munit_assert_string_equal(r.ok.s, "aello");

  stack_test_type_free(&stack);
  return MUNIT_OK;
}

MunitResult stack_test_type_empty_test(const MunitParameter *param, void *context) {
  stack_test_type stack = stack_test_type_create();
  munit_assert_true(stack_test_type_empty(&stack));

  stack_test_type_push(&stack, (test_type){.x = 4, .s = "hello"});
  munit_assert_false(stack_test_type_empty(&stack));

  stack_test_type_free(&stack);
  return MUNIT_OK;
}
