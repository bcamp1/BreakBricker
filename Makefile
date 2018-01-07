all:
	g++ -std=c++0x -c *.cpp
	g++ *.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
	./sfml-app
