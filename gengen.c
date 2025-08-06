#define GENGEN_IMPLEMENTATION
#include "gengen2.h"

int main() {
  ctemplate dynarray = template_create("dynarray");
  template_addfile(&dynarray, "dynarray.htpl", "dynarray_$T.h");
  template_addfile(&dynarray, "dynarray.ctpl", "dynarray_$T.c");
  template_addreplacement(&dynarray, "$T", NULL);
  template_addreplacement(&dynarray, "^T", NULL);
  template_addreplacement(&dynarray, "SUPPORT_1", NULL);
  template_addreplacement(&dynarray, "REALLOC", NULL);
  template_addreplacement(&dynarray, "CALLOC", NULL);

  replacement dynarray_repl = replacement_create();
  replacement_add(&dynarray_repl, "$T", "token");
  replacement_add(&dynarray_repl, "^T", "token");
  replacement_add(&dynarray_repl, "SUPPORT_1", "tokenizer.h");
  replacement_add(&dynarray_repl, "REALLOC", "realloc");
  replacement_add(&dynarray_repl, "CALLOC", "calloc");

  generator_run(gen_settings(.search_paths=paths("tpl"), .outdir="src"), dynarray, dynarray_repl);
}
