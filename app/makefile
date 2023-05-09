run: app
	cd src/interface/ && flask run

#######################################
### file

file.o: file.c file.h
	clang -c -Wall -Wextra -pedantic -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls file.c


app: 
	cd src/ && make main



run_debug: app
	cd src/interface/ && flask --app app.py --debug run


#######################################
### Astar

Astar: Astar.o
	clang -Wall -Wextra -pedantic -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls Astar.o -o Astar

Astar.o: Astar.c Astar.h
	clang -c -Wall -Wextra -pedantic -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls Astar.c