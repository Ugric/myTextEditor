build:
	rm -rf bin
	mkdir -p bin
	gcc -o bin/main *.c -lncurses