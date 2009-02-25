CC = gcc
CFLAGS = -g -Wall
EXEC = a2

$(EXEC): 
	$(CC) $(CFLAGS) -o $(EXEC) a2.c heap.c
clean:
	-rm $(EXEC)
	-rm *.exe
test:
	$(CC) $(CFLAGS) -o test test.c heap.c
cleantest:
	-rm test
	-rm *.exe
