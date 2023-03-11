build:
	gcc -std=c99 -o tema2 functiiCoada.c functiiStiva.c main.c -g

run:
	./tema2

clean:
	rm -f *.o tema2