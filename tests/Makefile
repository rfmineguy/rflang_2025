LIB_SOURCES := lib/munit.c
TEST_SOURCES := $(wildcard src/*.c)

LIBRFC_SOURCE_DIR := ../src/librfc
LIBRFC_SOURCE_FILES := $(wildcard $(LIBRFC_SOURCE_DIR)/*.c)
LIBRFC_FILES := $(LIBRFC_SOURCE_FILES)

LIBRFC_LIB_DIR := ../out/lib

.PHONY: getmunit
.PHONY: always clean
.PHONY: build_tests run_tests
always:
	mkdir -p out
	mkdir -p lib
clean:
	rm -rf out
	make -C .. clean

lib/munit.h:
	curl https://raw.githubusercontent.com/nemequ/munit/refs/heads/master/munit.h >> lib/munit.h
lib/munit.c:
	curl https://raw.githubusercontent.com/nemequ/munit/refs/heads/master/munit.c >> lib/munit.c

build_tests: always lib/munit.h lib/munit.c out/test_exe
out/test_exe: $(LIB_SOURCES) $(TEST_SOURCES) $(LIBRFC_FILES)
	make -C .. buildlib
	$(CC) -std=c17 $(LIB_SOURCES) $(TEST_SOURCES) $(LIBRFC_FILES) -o $@ -L$(LIBRFC_LIB_DIR) -lrfc

run_tests: build_tests
	./out/test_exe
