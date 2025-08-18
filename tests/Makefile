LIB_SOURCES := lib/munit.c
TEST_SOURCES := $(wildcard src/*.c)

TEST_GENERATED := generated/dynarray_token.c generated/stack_test_type.c

LIBRFC_SOURCE_DIR := ../src/librfc
LIBRFC_THIRDPARTY_DIR := ../src/thirdparty
LIBRFC_SOURCE_FILES := $(wildcard $(LIBRFC_SOURCE_DIR)/*.c)
LIBRFC_FILES := $(LIBRFC_SOURCE_FILES)

LIBRFC_LIB_DIR := ../out/lib

.PHONY: getmunit
.PHONY: always clean
.PHONY: build_tests run_tests
always:
	mkdir -p out
	mkdir -p lib
	mkdir -p generated
clean:
	rm -rf out
	rm -rf generated
	make -C .. clean

# PRE BUILD
$(TEST_GENERATED): prebuild_exe
	./prebuild_exe
prebuild_exe: gengen.c
	$(CC) -o $@ $^

lib/munit.h:
	curl https://raw.githubusercontent.com/nemequ/munit/refs/heads/master/munit.h >> lib/munit.h
lib/munit.c:
	curl https://raw.githubusercontent.com/nemequ/munit/refs/heads/master/munit.c >> lib/munit.c

build_tests: always $(TEST_GENERATED) lib/munit.h lib/munit.c out/test_exe
out/test_exe: $(LIB_SOURCES) $(TEST_SOURCES) $(LIBRFC_FILES) $(TEST_GENERATED)
	make -C .. buildlib
	$(CC) -std=c17 $(LIB_SOURCES) $(TEST_SOURCES) $(TEST_GENERATED) -o $@ -L$(LIBRFC_LIB_DIR)\
		-lrfc -I$(LIBRFC_THIRDPARTY_DIR) -I$(LIBRFC_SOURCE_DIR)\
		-Igenerated -I$(LIBRFC_TPL_SUPPORT_DIR) -I. -Isupport/tpl\
		-Isupport -DMUNIT_TEST_NAME_LEN=80

run_tests: build_tests
	./out/test_exe
