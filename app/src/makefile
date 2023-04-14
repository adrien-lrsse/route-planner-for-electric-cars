CC=clang
CFLAGS=-Wall -Wextra -pedantic -O0 -g3 -fsanitize=address
LIBS = -lsqlite3


main : main.o coordinates.o database_reader.o struct_mat_adj.o database_access.o  etape.o
	$(CC) $(CFLAGS) main.o coordinates.o database_reader.o struct_mat_adj.o database_access.o etape.o -o main $(LIBS)

main.o : main.c main.h borne.h coordinates.h database_reader.h database_access.h snow.h etape.h
	$(CC) $(CFLAGS) -c main.c



coordinates : coordinates.o coordinates.h 
	$(CC) $(CFLAGS) coordinates.o -o coordinates

coordinates.o : coordinates.c coordinates.h
	$(CC) $(CFLAGS) -c coordinates.c

database_access : database_access.o database_access.h  database_reader.h coordinates.h borne.h etape.h
	$(CC) $(CFLAGS) database_access.o -o database_access

database_access.o : database_access.c database_access.h etape.h database_reader.h borne.h etape.h
	$(CC) $(CFLAGS) -c database_access.c 

struct_mat_adj : struct_mat_adj.o struct_mat_adj.h
	$(CC) $(CFLAGS) struct_mat_adj.o -o struct_mat_adj
struct_mat_adj.o : struct_mat_adj.c struct_mat_adj.h
	$(CC) $(CFLAGS) -c struct_mat_adj.c




database_reader : database_reader.o database_reader.h
	$(CC) $(CFLAGS) database_reader.o -o database_reader $(LIBS)

database_reader.o : database_reader.c database_reader.h
	$(CC) $(CFLAGS) -c database_reader.c


etape.o : etape.h borne.h etape.c
	$(CC) $(CFLAGS) -c etape.c


clean :
	rm -f *.o coordinates database_reader struct_mat_adj main

create_mat_adj_in_txt : create_mat_adj_in_txt.o database_reader.h coordinates.h database_reader.o coordinates.o
	$(CC) $(CFLAGS)  database_reader.o coordinates.o create_mat_adj_in_txt.o -o create_mat_adj_in_txt $(LIBS)

create_mat_adj_in_txt.o : create_mat_adj_in_txt.c database_reader.h coordinates.h
	$(CC) $(CFLAGS) -c  create_mat_adj_in_txt.c

