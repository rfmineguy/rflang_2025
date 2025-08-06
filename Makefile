OUT := out
SRC := src
SOURCE_FILES := $(wildcard $(SRC)/*.c)


.PHONY: always clean build
always:
	mkdir -p $(OUT)
clean:
	rm -r $(OUT)
build: always $(OUT)/rfc


$(OUT)/rfc: $(SOURCE_FILES)
	$(CC) -o $@ $^
