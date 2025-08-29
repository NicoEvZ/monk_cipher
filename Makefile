CC = gcc
C_FLAGS = -Wall -g

monk_cipher: main.c
	$(CC) $(C_FLAGS) -o $@ $^