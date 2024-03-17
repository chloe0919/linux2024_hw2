CC = gcc
CFLAGS = -O1 -g -Wall 
LDFLAGS = -lrt

OBJS := main.o timsort.o list_sort.o

all: timtest list_sort

timtest: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lm
list_sort: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $< $(LDFLAGS)

clean:
	rm -f $(OBJS)
	rm -f *.dSYM
	rm -f timtest list_sort
	rm -f .*.o.d