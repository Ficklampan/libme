NAME = libme
VERSION = 2020

CC = clang++
BUILD = ./build
EXEC = libme
CFLAGS = 
C_CFLAGS = $(CFLAGS) -xc
CXX_CFLAGS = $(CFLAGS) -xc++ -g -Wall -Wextra -std=c++20 -I./include/libme
LFLAGS = 

EXTS = 
SRCS = ./src/memory.cpp \
	./src/filesystem.cpp
OBJS = $(SRCS:%=$(BUILD)/%.o)
DEPS = $(OBJS:%.o=%.d)

$(EXEC): $(OBJS) $(EXTS)
	ar rcs libme.a $^

-include $(DEPS)

$(BUILD)/./src/memory.cpp.o: ./src/memory.cpp
	@echo "compiling source './src/memory.cpp'"
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(CXX_CFLAGS) -MMD

$(BUILD)/./src/variant.cpp.o: ./src/variant.cpp
	@echo "compiling source './src/variant.cpp'"
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(CXX_CFLAGS) -MMD

$(BUILD)/./src/filesystem.cpp.o: ./src/filesystem.cpp
	@echo "compiling source './src/filesystem.cpp'"
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(CXX_CFLAGS) -MMD



.PHONY: clean
clean:
	rm -f $(EXEC) $(OBJS) $(DEPS)
