OUT := out
SRC := src
GENERATED := $(SRC)/dynarray_token.c $(SRC)/dynarray_token.h
SOURCE_FILES := $(wildcard $(SRC)/*.c) $(filter %.c, $(GENERATED))

.PHONY: prebuild
.PHONY: always clean build
always:
	mkdir -p $(OUT)
clean:
	rm -rf $(OUT)
	rm -f prebuild_exe
	rm -f $(GENERATED)


# PRE BUILD
$(GENERATED): prebuild_exe
	./prebuild_exe
prebuild_exe: gengen.c
	$(CC) -o $@ $^

# MAIN BUILD
build: always $(GENERATED) $(OUT)/rfc
$(OUT)/rfc: $(SOURCE_FILES)
	$(CC) -o $@ $^
