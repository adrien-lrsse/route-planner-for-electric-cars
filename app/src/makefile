coordinates : coordinates.o coordinates.h 
	clang -std=c99 -Wall -Wextra -pedantic -g3 -fsanitize=address coordinates.o -o coordinates


coordinates.o : coordinates.c coordinates.h
	clang -std=c99 -Wall -Wextra -pedantic -g3 -fsanitize=address -c coordinates.c



database_reader : database_reader.o database_reader.h
	clang -std=c99 -Wall -Wextra -pedantic -g3 -fsanitize=address database_reader.o -o database_reader -lsqlite3


database_reader.o : database_reader.c database_reader.h
	clang -std=c99 -Wall -Wextra -pedantic -g3 -fsanitize=address -c database_reader.c

clean :
	rm -f *.o coordinates database_reader