# Expects a prebuilt libswe.a from the Swiss ephemeris in the src directly.
# You can build it by unpacking the Swiss ephemeris and typing make libswe.a in the src directory

CFLAGS = -g -Wall -fPIC -I. -L. # for Linux and other gcc systems
OP=$(CFLAGS)  
CC=gcc	#for Linux

all: librise test

test: test.o
	$(CC) $(OP) -o test test.o -lm -ldl -lrise

librise: rise.o
	$(CC) $(OP) -shared -o librise.so rise.o -lswe -lm -ldl

clean:
	rm -f *.o librise.so test
	
rise.o:

test.o:
