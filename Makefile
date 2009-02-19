CC = gcc
CFLAGS = -g -Wall
EXEC = heap

$(EXEC): 
	$(CC) $(CFLAGS) -o $(EXEC) main.c heap.c
clean:
	-rm $(EXEC)
	-rm *.exe
test:
	$(CC) $(CFLAGS) -o test test.c heap.c
cleantest:
	-rm test
	-rm *.exe
