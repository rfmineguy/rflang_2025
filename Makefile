OUT := out
LIB_SRC := src/librfc
EXE_SRC := src/rfc
LIB_GENERATED := $(LIB_SRC)/dynarray_token.c $(LIB_SRC)/dynarray_token.h\
                 $(LIB_SRC)/stack_ast_node.c $(LIB_SRC)/stack_ast_node.h
LIB_SOURCE_FILES := $(wildcard $(LIB_SRC)/*.c) $(filter %.c, $(LIB_GENERATED))
EXE_SOURCE_FILES := $(EXE_SRC)/main.c

LIB_OBJECT_FILES := $(patsubst $(LIB_SRC)/%.c,$(OUT)/lib/%.o,$(LIB_SOURCE_FILES))

.PHONY: prebuild
.PHONY: always clean build buildlib buildexe
always:
	echo "$(LIB_GENERATED)"
	mkdir -p $(OUT)
	mkdir -p $(OUT)/lib
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
	$(CC) -o $@ $^ -L$(OUT)/lib -lrfc -I src/ -ggdb

# LIB BUILD
buildlib: always $(LIB_GENERATED) $(OUT)/lib/librfc.a
$(OUT)/lib/librfc.a: $(LIB_OBJECT_FILES)
	ar rcs $@ $^

$(OUT)/lib/%.o: $(LIB_SRC)/%.c
	$(CC) -c $< -o $@ -ggdb
