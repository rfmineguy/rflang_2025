# Build file for rflang compiler
# Table of Contents:
#   1. Variable declaration
#   2. Conditional compilation
#   3/4. Normal/Preprocess Build
# 		 a. librfc Build
# 	      i.   PHONY target declarations
# 	      ii.  Prebuild
# 	      iii. Build all
# 	      iv.  Build library
# 	   b. rfc Build
# 	      i.   Build exe




# == 1 (Variable declaration) ==
OUT := out
OUT_PREPROCESS := $(OUT)/preprocess
OUT_NORMAL := $(OUT)/normal
LIB_SRC := src/librfc
EXE_SRC := src/rfc
THIRDPARTY_SRC := src/thirdparty
LIB_GENERATED := $(LIB_SRC)/dynarray_token.c $(LIB_SRC)/dynarray_token.h\
                 $(LIB_SRC)/stack_ast_node.c $(LIB_SRC)/stack_ast_node.h
THIRDPARTY_SRC_FILES := $(THIRDPARTY_SRC)/arena_impl.c $(THIRDPARTY_SRC)/spall_util.c $(THIRDPARTY_SRC)/instrument.c
LIB_SOURCE_FILES := $(wildcard $(LIB_SRC)/*.c) $(filter %.c, $(LIB_GENERATED))
EXE_SOURCE_FILES := $(EXE_SRC)/main.c

LIB_OBJECT_FILES := $(patsubst $(LIB_SRC)/%.c,$(OUT)/lib/%.o,$(LIB_SOURCE_FILES))
THIRD_PARTY_OBJECT_FILES := $(patsubst $(THIRDPARTY_SRC)/%.c,$(OUT)/lib/thirdparty/%.o,$(THIRDPARTY_SRC_FILES))

LIB_PREPROCESS_FILES := $(patsubst $(LIB_SRC)/%.c,$(OUT_PREPROCESS)/lib/%.i,$(LIB_SOURCE_FILES))
THIRD_PARTY_PREPROCESS_FILES := $(patsubst $(THIRDPARTY_SRC)/%.c,$(OUT_PREPROCESS)/lib/thirdparty/%.i,$(THIRDPARTY_SRC_FILES))

LIB_PREPROCESS_OBJECT_FILES := $(patsubst $(OUT_PREPROCESS)/lib/%.i,$(OUT_PREPROCESS)/lib/objects/%.o,$(LIB_PREPROCESS_FILES))
THIRD_PARTY_PREPROCESS_OBJECT_FILES := $(patsubst $(OUT_PREPROCESS)/lib/thirdparty/%.i,$(OUT_PREPROCESS)/lib/thirdparty/objects/%.o,$(THIRD_PARTY_PREPROCESS_FILES))

# == 2 (Conditional compilation) ==
PROFILING ?= OFF
DEBUG ?= ON
CFLAGS :=
ifeq ($(PROFILING),ON)
	CFLAGS += -DENABLE_SPALL -finstrument-functions
endif
ifeq ($(DEBUG),ON)
	CFLAGS += -ggdb
endif

# ===============================================================
# == 3 (Normal Build) ===
# == 3a (librfc Build) ==
# == 3a.i (phony) ==
.PHONY: always clean build buildlib buildexe
always:
	echo "$(LIB_GENERATED)"
	mkdir -p $(OUT)
	mkdir -p $(OUT)/lib
	mkdir -p $(OUT)/lib/thirdparty
clean:
	rm -rf $(OUT)
	rm -f prebuild_exe
	rm -f $(LIB_GENERATED)

# == 3a.ii (Prebuild) ==
$(LIB_GENERATED): prebuild_exe
	./prebuild_exe
prebuild_exe: gengen.c
	$(CC) -o $@ $^

# == 3a.iii (Build all)
build: always $(LIB_GENERATED) $(OUT)/lib/librfc.a $(OUT)/rfc

# == 3a.iv (Build library)
buildlib: always $(LIB_GENERATED) $(OUT)/lib/librfc.a
$(OUT)/lib/librfc.a: $(LIB_OBJECT_FILES) $(THIRD_PARTY_OBJECT_FILES)
	ar rcs $@ $^

$(OUT)/lib/%.o: $(LIB_SRC)/%.c
	$(CC) -c $< -o $@ -ggdb -I $(THIRDPARTY_SRC) -Isrc/librfc -Isrc/tpl_support/ $(CFLAGS)

$(OUT)/lib/thirdparty/%.o: $(THIRDPARTY_SRC)/%.c
	$(CC) -c $< -o $@ -ggdb -I $(THIRDPARTY_SRC) -Isrc/librfc -Isrc/tpl_support/ $(CFLAGS)

# == 3b (rfc Build)
# == 3bi (Build exe)
buildexe: always $(OUT)/rfc $(LIB_GENERATED)
$(OUT)/rfc: $(EXE_SOURCE_FILES) $(OUT)/lib/librfc.a
	$(CC) -o $@ $^ -L$(OUT)/lib -lrfc -I src/ -I src/tpl_support/ -I $(THIRDPARTY_SRC) $(CFLAGS)


# ===============================================================
# == 4 (Preprocess Build
# == 3a (librfc Build) ==
# == 3a.i (phony) ==
.PHONY: always_preprocess clean_preprocess build_preprocess
always_preprocess:
	mkdir -p $(OUT_PREPROCESS)
	mkdir -p $(OUT_PREPROCESS)/lib/objects
	mkdir -p $(OUT_PREPROCESS)/lib/thirdparty/objects

clean_preprocess:
	rm -rf $(OUT_PREPROCESS)

# == 3a.ii (Prebuild)
#   Reused same target as above

# == 3a.iii (Build all)
build_preprocess: always_preprocess $(LIB_GENERATED) $(LIB_PREPROCESS_FILES) $(THIRD_PARTY_PREPROCESS_FILES) $(OUT_PREPROCESS)/rfc

# == 3a.iv (Build library)
build_preprocess_lib: always $(LIB_GENERATED) $(OUT_PREPROCESS)/lib/librfc.a
$(OUT_PREPROCESS)/lib/librfc.a: $(LIB_PREPROCESS_OBJECT_FILES) $(THIRD_PARTY_PREPROCESS_OBJECT_FILES)
	ar rcs $@ $^

$(OUT_PREPROCESS)/lib/objects/%.o: $(OUT_PREPROCESS)/lib/%.i
	$(CC) -c $< -o $@ -I $(THIRDPARTY_SRC) -Isrc/librfc -Isrc/tpl_support/ $(CFLAGS)
$(OUT_PREPROCESS)/lib/thirdparty/objects/%.o: $(OUT_PREPROCESS)/lib/thirdparty/%.i
	$(CC) -c $< -o $@ -I $(THIRDPARTY_SRC) -Isrc/librfc -Isrc/tpl_support/ $(CFLAGS)
$(OUT_PREPROCESS)/lib/thirdparty/%.i: $(THIRDPARTY_SRC)/%.c
	$(CC) -E -P $< -I $(THIRDPARTY_SRC) -Isrc/librfc -Isrc/tpl_support/ $(CFLAGS) | clang-format > $@
$(OUT_PREPROCESS)/lib/%.i:  $(LIB_SRC)/%.c $(EXE_SOURCE_FILES)
	$(CC) -E -P $< -I $(THIRDPARTY_SRC) -Isrc/librfc -Isrc/tpl_support/ $(CFLAGS) | clang-format > $@

# == 3b (rfc Build)
# == 3bi (Build exe)
$(OUT_PREPROCESS)/rfc: $(EXE_SOURCE_FILES) $(OUT_PREPROCESS)/lib/librfc.a
	$(CC) -o $@ $^ -L$(OUT_PREPROCESS)/lib -lrfc -I src/ -I src/tpl_support/ -I $(THIRDPARTY_SRC) -ggdb $(CFLAGS)
