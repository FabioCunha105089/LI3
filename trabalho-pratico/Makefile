OBJS	 = ArrayList.o Date.o Driver.o FileManager.o LinkedList.o Queries.o Ride.o User.o
OUT	 = programa-principal
SRC      = ./src/
CC	 = gcc
FLAGS	 = -g -c -Wall `pkg-config glib-2.0 --cflags` `pkg-config glib-2.0 --libs`
LFLAGS	 = `pkg-config glib-2.0 --cflags` `pkg-config glib-2.0 --libs`

all: $(OBJS)
	$(CC) `pkg-config glib-2.0 --cflags` `pkg-config glib-2.0 --libs` -g $(OBJS) -o $(OUT) $(LFLAGS) 
	
ArrayList.o: $(SRC)ArrayList.c
	$(CC) $(FLAGS) $(SRC)ArrayList.c

Date.o: $(SRC)Date.c
	$(CC) $(FLAGS) $(SRC)Date.c 

Driver.o: $(SRC)Driver.c
	$(CC) $(FLAGS) $(SRC)Driver.c 

FileManager.o: $(SRC)FileManager.c
	$(CC) $(FLAGS) $(SRC)FileManager.c 

LinkedList.o: $(SRC)LinkedList.c
	$(CC) $(FLAGS) $(SRC)LinkedList.c 

Queries.o: $(SRC)Queries.c
	$(CC) $(FLAGS) $(SRC)Queries.c 

Ride.o: $(SRC)Ride.c
	$(CC) $(FLAGS) $(SRC)Ride.c 

User.o: $(SRC)User.c
	$(CC) $(FLAGS) $(SRC)User.c 

clean:
	rm -f $(OBJS) $(OUT)
