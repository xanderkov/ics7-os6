CC = g++
BINARES = $(patsubst %.cpp,bin/%.o, $(wildcard *.cpp))
FLAGS = -Wall -g -Werror -std=c++17

app: $(BINARES)
	$(CC) $^ -o $@ $(GTK_FLAGS)

bin/%.o: %.cpp
	mkdir -p bin
	$(CC) -c -o $@ $(GTK_FLAGS) $(FLAGS) $^ 

clean:
	rm bin/*.o app