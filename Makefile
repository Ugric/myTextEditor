full:
	make clean
	make build

build:
	mkdir -p bin
	gcc -o bin/mte *.c -lncurses

clean:
	rm -rf bin