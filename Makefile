CC := gcc
FILE := kavpass.c
CFLAGS := -march=native -O3 
default:
	$(CC) $(CFLAGS) -o kavpass $(FILE)
