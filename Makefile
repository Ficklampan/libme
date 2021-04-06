NAME = Libme
VERSION = 2020
CC = clang++
BUILD = ./build
OUT = libme
INCD = -I./include/libme
LIBD = 
LIBS = 
CFLAGS = $(INCD)
C_CFLAGS = $(CFLAGS) -xc 
CXX_CFLAGS = $(CFLAGS) -xc++ -g -Wall -Wextra -std=c++20
OBJC_CFLAGS = $(CFLAGS) -xobjc 
LFLAGS = $(LIBD) $(LIBS) 
SRCS = ./src/memory.cpp \
	./src/filesystem.cpp
OBJS = $(SRCS:%=$(BUILD)/%.o)
DEPS = $(OBJS:%.o=%.d)

EXTS = 

.PHONY: $(NAME)
$(NAME): $(OUT) $(EXTS)

$(OUT): $(OBJS)
	ar rcs libme.a $^

-include $(DEPS)

$(BUILD)/./src/memory.cpp.o: ./src/memory.cpp
	@echo "compiling source '$<'"
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(CXX_CFLAGS) -MMD

$(BUILD)/./src/filesystem.cpp.o: ./src/filesystem.cpp
	@echo "compiling source '$<'"
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(CXX_CFLAGS) -MMD

$(BUILD)/./src/ios.cpp.o: ./src/ios.cpp
	@echo "compiling source '$<'"
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(CXX_CFLAGS) -MMD

$(BUILD)/./src/vsprintf.cpp.o: ./src/vsprintf.cpp
	@echo "compiling source '$<'"
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(CXX_CFLAGS) -MMD




.PHONY: install
install:

.PHONY: clean
clean:
	rm -f $(OUT) $(OBJS) $(DEPS)
