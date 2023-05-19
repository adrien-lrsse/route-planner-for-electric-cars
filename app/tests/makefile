CC=clang
CFLAGS=-Wall -Wextra -pedantic -O0 -g3 -fsanitize=address
LIBS = -lsqlite3 
NO_DEBUG = -DSUPPRESS_PRINTFS

ALL = main coordinates itinerary struct_mat_adj database_reader create_mat_adj_in_txt borne etape vehicules export simulation

DEPENDENCIES = database_reader.c coordinates.c borne.c etape.c vehicules.c itinerary.c thread.c random_points.c export.c simulation.c
dot=..


tests: $(patsubst %.c,$(dot)/src/%.o,$(DEPENDENCIES)) tests.o $(LIBS)
	$(CC) $(CFLAGS) $(patsubst %.c,$(dot)/src/%.o,$(DEPENDENCIES)) tests.o -o tests $(LIBS)

tests.o : tests.c $(wildcard ../src/*.h)


