BIN=../bin
INCdir=../include
FLAGS=-O2 -I$(INCdir)
SUBdir=components core

.PHONY: all $(SUBdir) clean
all: gomoku

gomoku: $(SUBdir)
	mkdir -p ../bin
	g++ main.cpp */*.o $(FLAGS) -o $(BIN)/gomoku

$(SUBdir):
	$(MAKE) -C $@

clean:
	$(MAKE) -C components clean
	$(MAKE) -C core clean