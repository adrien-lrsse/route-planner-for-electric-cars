CC=clang
CFLAGS=-Wall -Wextra -pedantic -O0 -g3 -fsanitize=address
LIBS = -lsqlite3 -pthread
#NO_DEBUG = -DSUPPRESS_PRINTFS

ALL = main coordinates itinerary struct_mat_adj database_reader create_mat_adj_in_txt borne etape vehicules export simulation

main : main.o coordinates.o database_reader.o struct_mat_adj.o itinerary.o etape.o export.o vehicules.o borne.o thread.o
	$(CC) $(CFLAGS) main.o coordinates.o database_reader.o struct_mat_adj.o itinerary.o etape.o export.o vehicules.o borne.o thread.o -o main $(LIBS)

main.o : main.c main.h borne.h coordinates.h database_reader.h itinerary.h snow.h etape.h export.h vehicules.h borne.h thread.h
	$(CC) $(CFLAGS) -c main.c

thread.o : thread.c thread.h
	$(CC) $(CFLAGS) -c thread.c

export.o : export.c export.h etape.h
	$(CC) $(CFLAGS) -c export.c $(NO_DEBUG)

coordinates : coordinates.o coordinates.h
	$(CC) $(CFLAGS) coordinates.o -o coordinates

coordinates.o : coordinates.c coordinates.h
	$(CC) $(CFLAGS) -c coordinates.c $(NO_DEBUG)

itinerary : itinerary.o itinerary.h  database_reader.h coordinates.h borne.h etape.h vehicules.o database_reader.o
	$(CC) $(CFLAGS) database_access.o vehicules.o -o database_access

itinerary.o : itinerary.c itinerary.h etape.h database_reader.h borne.h etape.h vehicules.h
	$(CC) $(CFLAGS) -c itinerary.c $(NO_DEBUG)

simulation : simulation.h simulation.o database_reader.o vehicules.o coordinates.o vehicules.h database_reader.h coordinates.h itinerary.o itinerary.h etape.o etape.h
	$(CC) $(CFLAGS) database_reader.o vehicules.o coordinates.o simulation.o itinerary.o etape.o -o simulation $(LIBS)

simulation.o : simulation.h simulation.c vehicules.h database_reader.h coordinates.h itinerary.h etape.h
	$(CC) $(CFLAGS) -c simulation.c $(NO_DEBUG)

struct_mat_adj : struct_mat_adj.o struct_mat_adj.h
	$(CC) $(CFLAGS) struct_mat_adj.o -o struct_mat_adj

struct_mat_adj.o : struct_mat_adj.c struct_mat_adj.h
	$(CC) $(CFLAGS) -c struct_mat_adj.c $(NO_DEBUG)

vehicules : vehicules.o vehicules.h database_reader.h database_reader.o borne.h
	$(CC) $(CFLAGS) vehicules.o database_reader.o -o vehicules $(LIBS)

vehicules.o : vehicules.c vehicules.h database_reader.h borne.h
	$(CC) $(CFLAGS) -c vehicules.c $(NO_DEBUG)


database_reader : database_reader.o database_reader.o
	$(CC) $(CFLAGS) database_reader.o -o database_reader $(LIBS)

database_reader.o : database_reader.c database_reader.h
	$(CC) $(CFLAGS) -c database_reader.c $(NO_DEBUG)

borne.o : borne.c borne.h etape.h
	$(CC) $(CFLAGS) -c borne.c $(NO_DEBUG)

etape.o : etape.h borne.h etape.c
	$(CC) $(CFLAGS) -c etape.c $(NO_DEBUG)


clean :
	rm -f *.o $(ALL)

create_mat_adj_in_txt : create_mat_adj_in_txt.o database_reader.h coordinates.h database_reader.o coordinates.o
	$(CC) $(CFLAGS)  database_reader.o coordinates.o create_mat_adj_in_txt.o -o create_mat_adj_in_txt $(LIBS)

create_mat_adj_in_txt.o : create_mat_adj_in_txt.c database_reader.h coordinates.h
	$(CC) $(CFLAGS) -c  create_mat_adj_in_txt.c $(NO_DEBUG)
