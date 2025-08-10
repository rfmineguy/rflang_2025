#define GENGEN_IMPLEMENTATION
#include "gengen2.h"

int main() {
  /*
   * TEMPLATES
   */
  ctemplate dynarray = template_create("dynarray");
  template_addfile(&dynarray, "dynarray.htpl", "dynarray_$T.h");
  template_addfile(&dynarray, "dynarray.ctpl", "dynarray_$T.c");
  template_addreplacement(&dynarray, "$T", NULL);
  template_addreplacement(&dynarray, "^T", NULL);
  template_addreplacement(&dynarray, "SUPPORT_1", NULL);
  template_addreplacement(&dynarray, "REALLOC", NULL);
  template_addreplacement(&dynarray, "CALLOC", NULL);
  template_addreplacement(&dynarray, "FREE", NULL);

  ctemplate stack = template_create("stack");
  template_addfile(&stack, "stack.htpl", "stack_$T.h");
  template_addfile(&stack, "stack.ctpl", "stack_$T.c");
  template_addreplacement(&stack, "$T", NULL);
  template_addreplacement(&stack, "^T", NULL);
  template_addreplacement(&stack, "SUPPORT_1", NULL);
  template_addreplacement(&stack, "REALLOC", NULL);
  template_addreplacement(&stack, "CALLOC", NULL);
  template_addreplacement(&stack, "FREE", NULL);

  /*
   * REPLACEMENTS
   */
  replacement token_dynarray_repl = replacement_create();
  replacement_add(&token_dynarray_repl, "$T", "token");
  replacement_add(&token_dynarray_repl, "^T", "token");
  replacement_add(&token_dynarray_repl, "SUPPORT_1", "token.h");
  replacement_add(&token_dynarray_repl, "REALLOC", "realloc");
  replacement_add(&token_dynarray_repl, "CALLOC", "calloc");
  replacement_add(&token_dynarray_repl, "FREE", "free");
  
  replacement parser_stack_repl = replacement_create();
  replacement_add(&parser_stack_repl, "$T", "ast_node");
  replacement_add(&parser_stack_repl, "^T", "variant_ast_node");
  replacement_add(&parser_stack_repl, "SUPPORT_1", "ast.h");
  replacement_add(&parser_stack_repl, "REALLOC", "realloc");
  replacement_add(&parser_stack_repl, "CALLOC", "calloc");
  replacement_add(&parser_stack_repl, "FREE", "free");

  /*
   * GENERATORS
   */
  generator_run(gen_settings(.search_paths=paths("tpl"), .outdir="src/librfc"), dynarray, token_dynarray_repl);
  generator_run(gen_settings(.search_paths=paths("tpl"), .outdir="src/librfc"), stack,    parser_stack_repl);
}
