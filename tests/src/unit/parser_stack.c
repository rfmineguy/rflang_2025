#include "tests.h"
#include "arena.h"
#include "../../src/librfc/parser.h"
#include "../../src/librfc/variant.h"
#define arena_alloc_(size) arena_alloc(&a, size)

/*
 * Test to make sure that the elements returned from `stack_check` match those that are checked
 */
MunitResult parser_stack_check_test_1(const MunitParameter *param, void *context){
  stack_ast_node stack = stack_ast_node_create();
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = ID, .len = 1, .start = "x" })));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = EQ, .len = 1, .start = "=" })));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = EQ, .len = 1, .start = "42" })));

  match(stack_check(&stack, check_seq({token(ID), token(EQ), token(EQ)})), stack_check, {
    munit_assert_int(result_.ok.count, ==, 3);
    match_variant(result_.ok.nodes[0], ast_node, {
      variant_case(ast_node, Token, {
        munit_assert_int(v.Token.t.type, ==, EQ);
        munit_assert_int(v.Token.t.len, ==, 1);
        munit_assert_string_equal(v.Token.t.start, "42");
      })
      variant_default({ munit_assert(false); })
    })
    match_variant(result_.ok.nodes[1], ast_node, {
      variant_case(ast_node, Token, { 
        munit_assert_int(v.Token.t.type, ==, EQ);
        munit_assert_int(v.Token.t.len, ==, 1);
        munit_assert_string_equal(v.Token.t.start, "=");
      })
      variant_default({ munit_assert(false); })
    })
    match_variant(result_.ok.nodes[2], ast_node, {
      variant_case(ast_node, Token, {
        munit_assert_int(v.Token.t.type, ==, ID);
        munit_assert_int(v.Token.t.len, ==, 1);
        munit_assert_string_equal(v.Token.t.start, "x");
      })
      variant_default({ munit_assert(false); })
    })
  }, {
    munit_logf(1, "%s", result_.err);
    munit_assert(false);
  });

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult parser_stack_check_test_2(const MunitParameter *param, void *context){
  Arena a = {};
  stack_ast_node stack = stack_ast_node_create();

  variant_ast_vardecl* vardecl_ = make_variant_alloc(ast_vardecl, arena_alloc_);
  variant_ast_type* type_ = make_variant_alloc(ast_type, arena_alloc_);
  *type_ = make_variant(ast_type, Id, ((type_id){.id = (token) {.type = ID, .len = 3, .start = "boo"}}));
  *vardecl_ = make_variant(ast_vardecl, Main, ((vardecl) { .type = type_, .id = (token) { .type = ID, .len = 5, .start = "" } }));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = ID, .len = 1, .start = "x" })));
  stack_ast_node_push(&stack, make_variant(ast_node, VariantVar, vardecl_));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = EQ, .len = 1, .start = "42" })));

  match(stack_check(&stack, check_seq({token(ID), ast(variant_ast_node_type_VariantVar), token(EQ)})), stack_check, {
    munit_assert_int(result_.ok.count, ==, 3);
    match_variant(result_.ok.nodes[0], ast_node, {
      variant_case(ast_node, Token, {
        munit_assert_int(v.Token.t.type, ==, EQ);
        munit_assert_int(v.Token.t.len, ==, 1);
        munit_assert_string_equal(v.Token.t.start, "42");
      })
      variant_default({ munit_assert(false); })
    })
    match_variant(result_.ok.nodes[1], ast_node, {
      variant_case(ast_node, VariantVar, { 
        munit_assert_int(v.VariantVar->Main.type->type,          ==, variant_ast_type_type_Id);
        munit_assert_int(v.VariantVar->Main.type->Id.id.len,     ==, 3);
        munit_assert_string_equal(v.VariantVar->Main.type->Id.id.start, "boo");
      })
      variant_default({ munit_assert(false); })
    })
    match_variant(result_.ok.nodes[2], ast_node, {
      variant_case(ast_node, Token, {
        munit_assert_int(v.Token.t.type, ==, ID);
        munit_assert_int(v.Token.t.len, ==, 1);
        munit_assert_string_equal(v.Token.t.start, "x");
      })
      variant_default({ munit_assert(false); })
    })
  }, {
    munit_assert(false);
  });

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult parser_stack_check_passed_limit_test(const MunitParameter *param, void *context){
  stack_ast_node stack = stack_ast_node_create();
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) {.type = ID, .len = 1, .start = "x" })));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) {.type = EQ, .len = 1, .start = "=" })));

  // We expect this check to fail given the stack only has 2 items, but we check for 3
  match(stack_check(&stack, check_seq({token(ID), token(EQ), token(INTLIT)})), stack_check, {
    munit_assert(false);
  },{
    munit_assert(true);
  });

  stack_ast_node_free(&stack);
  return MUNIT_OK;
}

MunitResult parser_stack_check_stack_larger_than_check(const MunitParameter *param, void *context){
  Arena a = {};
  stack_ast_node stack = stack_ast_node_create();
  variant_ast_vardecl* vardecl_ = make_variant_alloc(ast_vardecl, arena_alloc_);
  variant_ast_type* type_ = make_variant_alloc(ast_type, arena_alloc_);
  *type_ = make_variant(ast_type, Id, ((type_id){.id = (token) {.type = ID, .len = 3, .start = "boo"}}));
  *vardecl_ = make_variant(ast_vardecl, Main, ((vardecl) { .type = type_, .id = (token) { .type = ID, .len = 5, .start = "" } }));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = EQ, .len = 1, .start = "42" })));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = ID, .len = 1, .start = "x" })));
  stack_ast_node_push(&stack, make_variant(ast_node, VariantVar, vardecl_));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = EQ, .len = 1, .start = "42" })));

  match(stack_check(&stack, check_seq({token(ID), ast(variant_ast_node_type_VariantVar), token(EQ)})), stack_check, {
    munit_assert_int(result_.ok.count, ==, 3);
    match_variant(result_.ok.nodes[0], ast_node, {
      variant_case(ast_node, Token, {
        munit_assert_int(v.Token.t.type, ==, EQ);
        munit_assert_int(v.Token.t.len, ==, 1);
        munit_assert_string_equal(v.Token.t.start, "42");
      })
      variant_default({ munit_assert(false); })
    })
    match_variant(result_.ok.nodes[1], ast_node, {
      variant_case(ast_node, VariantVar, { 
        munit_assert_int(v.VariantVar->Main.type->type,          ==, variant_ast_type_type_Id);
        munit_assert_int(v.VariantVar->Main.type->Id.id.len,     ==, 3);
        munit_assert_string_equal(v.VariantVar->Main.type->Id.id.start, "boo");
      })
      variant_default({ munit_assert(false); })
    })
    match_variant(result_.ok.nodes[2], ast_node, {
      variant_case(ast_node, Token, {
        munit_assert_int(v.Token.t.type, ==, ID);
        munit_assert_int(v.Token.t.len, ==, 1);
        munit_assert_string_equal(v.Token.t.start, "x");
      })
      variant_default({ munit_assert(false); })
    })
  }, {
    munit_assert(false);
  });

  stack_ast_node_free(&stack);
  arena_free(&a);
  return MUNIT_OK;
}

MunitResult parser_stack_check_incorrect_ast_node(const MunitParameter *param, void *context) {
  Arena a = {};
  stack_ast_node stack = stack_ast_node_create();
  variant_ast_vardecl* vardecl_ = make_variant_alloc(ast_vardecl, arena_alloc_);
  variant_ast_type* type_ = make_variant_alloc(ast_type, arena_alloc_);
  *type_ = make_variant(ast_type, Id, ((type_id){.id = (token) {.type = ID, .len = 3, .start = "boo"}}));
  *vardecl_ = make_variant(ast_vardecl, Main, ((vardecl) { .type = type_, .id = (token) { .type = ID, .len = 5, .start = "" } }));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = EQ, .len = 1, .start = "42" })));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = ID, .len = 1, .start = "x" })));
  stack_ast_node_push(&stack, make_variant(ast_node, VariantVar, vardecl_));
  stack_ast_node_push(&stack, make_variant(ast_node, Token, ((token) { .type = EQ, .len = 1, .start = "42" })));

  match(stack_check(&stack, check_seq({token(ID), ast(variant_ast_node_type_VariantVar), token(EQ)})), stack_check, {
    munit_assert_int(result_.ok.count, ==, 3);
    match_variant(result_.ok.nodes[0], ast_node, {
      variant_case(ast_node, Token, {
        munit_assert_int(v.Token.t.type, ==, EQ);
        munit_assert_int(v.Token.t.len, ==, 1);
        munit_assert_string_equal(v.Token.t.start, "42");
      })
      variant_default({ munit_assert(false); })
    })
    match_variant(result_.ok.nodes[1], ast_node, {
      variant_case(ast_node, VariantVar, { 
        munit_assert_int(v.VariantVar->Main.type->type,          ==, variant_ast_type_type_Id);
        munit_assert_int(v.VariantVar->Main.type->Id.id.len,     ==, 3);
        munit_assert_string_equal(v.VariantVar->Main.type->Id.id.start, "boo");
      })
      variant_default({ munit_assert(false); })
    })
    match_variant(result_.ok.nodes[2], ast_node, {
      variant_case(ast_node, Token, {
        munit_assert_int(v.Token.t.type, ==, ID);
        munit_assert_int(v.Token.t.len, ==, 1);
        munit_assert_string_equal(v.Token.t.start, "x");
      })
      variant_default({ munit_assert(false); })
    })
  }, {
    munit_assert(false);
  });

  stack_ast_node_free(&stack);
  arena_free(&a);
  return MUNIT_OK;
}
