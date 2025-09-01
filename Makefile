# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Project name
TARGET = monk_cipher.exe

# Source files
SRCS = main.c ciphers.c 

# Object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files into final binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each .c into .o
%.o: %.c ciphers.h
	$(CC) $(CFLAGS) -c $<

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)