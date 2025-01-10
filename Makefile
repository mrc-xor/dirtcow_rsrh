CC=gcc
CFLAGS=-pthread

memc0w:memc0w.o
	$(CC) memc0w.o $(CFLAGS) -o memc0w
memc0w.o:memc0w.c
	$(CC) -c memc0w.c $(CFLAGS) -o memc0w.o
clean:
	rm -f memc0w.o memc0w
