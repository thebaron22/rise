# Expects a prebuilt libswe.a from the Swiss ephemeris in the src directly.
# You can build it by unpacking the Swiss ephemeris and typing make libswe.a in the src directory

CFLAGS = -g -Wall -fPIC -Isrc -Lsrc # for Linux and other gcc systems
OP=$(CFLAGS)  
CC=gcc	#for Linux

rise: rise.o
	$(CC) $(OP) -o rise rise.o -l:libswe.a -lm -ldl

clean:
	rm -f *.o rise src/*.o src/swetest src/libswe*
	
rise.o:
