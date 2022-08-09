CC := gcc
BUILD_DIR := ./build
SRC_DIR := ./src
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%.c=%.o)
RM := rm -rf
CFLAGS := -march=native -O2 -Wall -Wextra
TARGET := kavpass

$(BUILD_DIR): ${SRCS}
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $(BUILD_DIR)/$(notdir $(OBJS))

clean:
	$(RM) $(BUILD_DIR)

install:
	$(CC) $(BUILD_DIR)/$(notdir $(OBJS)) -o $(BUILD_DIR)/$(TARGET)
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${BUILD_DIR}/${TARGET} ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/kavpass

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/kavpass
