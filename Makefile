CC     = gcc
CFLAGS = -Wall -g -std=c99
CCLINK = $(CC)
OBJS   = grades.o 
EXEC  =  libgrades.so
RM = rm -fr *.o libgrades.so 

libgrades.so: grades.o
	$(CCLINK) -shared grades.o -o libgrades.so -llinked-list -L.

grades.o: grades.c grades.h linked-list.h
	$(CC) $(CFLAGS) -c -fpic grades.c


clean:
	$(RM)

