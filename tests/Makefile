LIB_SOURCES := lib/munit.c
TEST_SOURCES := $(wildcard src/*.c)

.PHONY: getmunit
.PHONY: always clean
.PHONY: build_tests run_tests
always:
	mkdir -p out
	mkdir -p lib
clean:
	rm -rf out

lib/munit.h:
	curl https://raw.githubusercontent.com/nemequ/munit/refs/heads/master/munit.h >> lib/munit.h
lib/munit.c:
	curl https://raw.githubusercontent.com/nemequ/munit/refs/heads/master/munit.c >> lib/munit.c

build_tests: always lib/munit.h lib/munit.c out/test_exe
out/test_exe: $(LIB_SOURCES) $(TEST_SOURCES)
	$(CC) -std=c17 $^ -o $@

run_tests: build_tests
	./out/test_exe
