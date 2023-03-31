coordinates : coordinates.o coordinates.h 
	clang -std=c99 -Wall -Wextra -pedantic -g3 -fsanitize=address coordinates.o -o coordinates


coordinates.o : coordinates.c coordinates.h
	clang -std=c99 -Wall -Wextra -pedantic -g3 -fsanitize=address -c coordinates.c