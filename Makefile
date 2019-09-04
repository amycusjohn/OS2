dt: main.o
	gcc main.o -o dt
main.o: main.c
	gcc -c main.c
clean:
	rm *.o dt
