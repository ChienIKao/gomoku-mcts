INCdir=../../include
FLAGS=-O2 -I$(INCdir)
OBJ=agent.o

.PHONY: clean
all: $(OBJ)

%.o: %.cpp
	g++ $^ -c $(FLAGS) -o $@

clean:
	-rm -rf *.o