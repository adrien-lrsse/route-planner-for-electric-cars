CC=clang
CFLAGS=-Wall -Wextra -pedantic -O0 -g3 -fsanitize=address
LIBS = -lsqlite3 
NO_DEBUG = -DSUPPRESS_PRINTFS

ALL = main coordinates itinerary struct_mat_adj database_reader create_mat_adj_in_txt borne etape vehicules export simulation

depandenccies = database_reader.c coordinates.c borne.c etape.c vehicules.c itinerary.c
dot=..


tests.o : tests.c $(wildcard ../src/*.h)

tests :  $(wildcard ../src/*.o) tests.o $(LIBS)

tests2: $(patsubst %.c,$(dot)/src/%.o,$(depandenccies)) tests.o $(LIBS)
	$(CC) $(CFLAGS) $(patsubst %.c,$(dot)/src/%.o,$(depandenccies)) tests.o -o tests $(LIBS)

tests3: $(addprefix $(dot)/src/,$(patsubst %.c,%.o,$(depandenccies)))