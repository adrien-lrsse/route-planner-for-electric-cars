run: app
	ulimit -n 1048576
	cd src/interface/ && ulimit -n 1048576 && flask run

#######################################
### file

file.o: file.c file.h
	clang -c -Wall -Wextra -pedantic -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls file.c


app: 
	cd src/ && make main

clean : 
	cd src/ && make clean

run_debug: app
	cd src/interface/ && flask --app app.py --debug run


#######################################
### Astar

Astar: Astar.o
	clang -Wall -Wextra -pedantic -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls Astar.o -o Astar

Astar.o: Astar.c Astar.h
	clang -c -Wall -Wextra -pedantic -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls Astar.c