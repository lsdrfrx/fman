DEFAULT: build run

build:
	g++ main.cpp -lncursesw -o bin/main

run: build
	./bin/main
