OBJS	 = ArrayList.o Date.o Driver.o FileManager.o LinkedList.o Queries.o Ride.o User.o Main.o
OBJS_T   = ArrayList.o Date.o Driver.o FileManager.o LinkedList.o Queries.o Ride.o User.o Testes.o
OUT	 	 = programa-principal
OUT_T    = programa-testes
SRC      = ./src/
CC	 	 = gcc
FLAGS	 = -g -c -Wall -Iinclude `pkg-config glib-2.0 --cflags` `pkg-config glib-2.0 --libs`
LFLAGS	 = `pkg-config glib-2.0 --cflags` `pkg-config glib-2.0 --libs` -lm

all: $(OBJS) $(OBJS_T)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)
	$(CC) -g $(OBJS_T) -o $(OUT_T) $(LFLAGS)

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
	
Main.o: $(SRC)Main.c Resultados
	$(CC) $(FLAGS) $(SRC)Main.c

Testes.o: $(SRC)Testes.c
	$(CC) $(FLAGS) $(SRC)Testes.c

Resultados: 
	mkdir Resultados
clean:
	rm -f -r $(OBJS) $(OUT) Resultados
