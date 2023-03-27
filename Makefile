CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99

all: q2

gale_shapley: q2.c
	$(CC) $(CFLAGS) q2.c -o q2

clean:
	rm -f q2
