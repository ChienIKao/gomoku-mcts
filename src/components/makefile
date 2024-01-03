INCdir=../../include
FLAGS=-O2 -I$(INCdir)
OBJ=board.o node.o

.PHONY: clean
all: $(OBJ)

%.o: %.cpp
	g++ $^ -c $(FLAGS) -o $@

clean:
	-rm -rf *.o