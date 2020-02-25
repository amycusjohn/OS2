CC = gcc
CFLAGS = -g
TARGET = oss prime

all: oss prime

oss: oss.o
	$(CC) -o oss oss.o

prime: child.o
	$(CC) -o  prime child.o
oss.o: oss.c
	$(CC) -c $(CFLAGS) oss.c
prime.o: child.c
	$(CC) -c $(CFLAGS) child.c

clean:
	rm -f *.o oss prime
