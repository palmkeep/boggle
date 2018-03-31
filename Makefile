CC=c++ -std=c++11
CFLAGS= -lncurses -lpthread -Wall
DEPS=
OBJ=toggle.cpp

toggle: $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

.PHONY: clean

clean:
	rm -f toggle
