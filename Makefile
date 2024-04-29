build:
	rm -rf bin
	mkdir -p bin
	clang -o bin/main *.c -lncurses