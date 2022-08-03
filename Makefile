CC := gcc
FILE := kavpass.c
CFLAGS := -march=native -Os -Wall
default:
	$(CC) $(CFLAGS) -o kavpass $(FILE)
