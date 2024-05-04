full:
	make clean
	make build

build:
	mkdir -p bin
	gcc -o bin/mte *.c -lncurses -Wall -Wextra -pedantic -std=c99 -Wimplicit-fallthrough=0

clean:
	rm -rf bin