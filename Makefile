OUT := out
SRC := src
SOURCE_FILES := $(wildcard $(SRC)/*.c)


.PHONY: prebuild
.PHONY: always clean build
always:
	mkdir -p $(OUT)
clean:
	-rm -r $(OUT)
	rm prebuild_exe
prebuild: always prebuild_exe
build: prebuild $(OUT)/rfc
	./prebuild_exe

prebuild_exe: gengen.c
	$(CC) -o $@ $^

$(OUT)/rfc: $(SOURCE_FILES)
	$(CC) -o $@ $^
