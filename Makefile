OUT := out
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

PROFILING ?= OFF

CFLAGS :=
ifeq ($(PROFILING),ON)
	CFLAGS += -DENABLE_SPALL -finstrument-functions
else
	CFLAGS +=
endif

.PHONY: prebuild
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

# PRE BUILD
$(LIB_GENERATED): prebuild_exe
	./prebuild_exe
prebuild_exe: gengen.c
	$(CC) -o $@ $^

build: always $(LIB_GENERATED) $(OUT)/lib/librfc.a $(OUT)/rfc

# MAIN BUILD
buildexe: always $(OUT)/rfc $(LIB_GENERATED) $(OUT)/lib/librfc.a
$(OUT)/rfc: $(EXE_SOURCE_FILES)
	$(CC) -o $@ $^ -L$(OUT)/lib -lrfc -I src/ -I src/tpl_support/ -I $(THIRDPARTY_SRC) -ggdb $(CFLAGS)

# LIB BUILD
buildlib: always $(LIB_GENERATED) $(OUT)/lib/librfc.a
$(OUT)/lib/librfc.a: $(LIB_OBJECT_FILES) $(THIRD_PARTY_OBJECT_FILES)
	ar rcs $@ $^

$(OUT)/lib/%.o: $(LIB_SRC)/%.c
	$(CC) -c $< -o $@ -ggdb -I $(THIRDPARTY_SRC) -Isrc/librfc -Isrc/tpl_support/ $(CFLAGS)

$(OUT)/lib/thirdparty/%.o: $(THIRDPARTY_SRC)/%.c
	$(CC) -c $< -o $@ -ggdb -I $(THIRDPARTY_SRC) -Isrc/librfc -Isrc/tpl_support/ $(CFLAGS)
