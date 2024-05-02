full:
	make clean
	make build

build:
	mkdir -p bin
	gcc -o bin/mte *.c -lncurses -Wall -Wextra -Werror -pedantic -std=c99

clean:
	rm -rf bin