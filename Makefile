clean:
	rm -rf bin

build:
	mkdir -p bin
	gcc -o bin/main *.c -lncurses