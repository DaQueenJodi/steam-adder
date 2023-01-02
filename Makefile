SRCS=$(shell find -name "*.c")
BIN=bin/steam-adder
CC=clang
DEPS=gtk+-3.0

LDFLAGS=$(shell pkgconf --libs $(DEPS)) $(SAN)
CFLAGS=$(shell pkgconf --cflags $(DEPS)) -Wextra -Wall -pedantic -ggdb $(SAN)

all: bin
	$(CC) $(SRCS) $(CFLAGS) $(LDFLAGS) -o $(BIN)


bin:
	mkdir bin 

run: $(BIN)
	$(BIN)
