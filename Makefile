OUT := out
SRC := src
GENERATED := $(SRC)/dynarray_token.c $(SRC)/dynarray_token.h
SOURCE_FILES := $(wildcard $(SRC)/*.c) $(filter %.c, $(GENERATED))

.PHONY: prebuild
.PHONY: always clean build
always:
	echo "$(GENERATED)"
	mkdir -p $(OUT)
clean:
	-rm -r $(OUT)
	rm prebuild_exe


# PRE BUILD
$(GENERATED): prebuild_exe
	./prebuild_exe
prebuild_exe: gengen.c
	$(CC) -o $@ $^

# MAIN BUILD
build: always $(GENERATED) $(OUT)/rfc
$(OUT)/rfc: $(SOURCE_FILES)
	$(CC) -o $@ $^
