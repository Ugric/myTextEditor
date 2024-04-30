build:
	mkdir -p bin
	gcc -o bin/main *.c -lncurses

clean:
	rm -rf bin