FLAGS = -O3 -g
10marta : main.o tron.o dfs.o
	g++ $^ -o $@

main.o : main.cpp map.h
	g++ $(FLAGS) -c main.cpp 

tron.o : tron.cpp map.h
	g++ $(FLAGS) -c tron.cpp 

dfs.o : dfs.cpp map.h
	g++ $(FLAGS) -c dfs.cpp

clean : 
	rm -f tron.o main.o bfs.o dfs.o mojbot