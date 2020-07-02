20151550.out : p1.o p2.o p3.o
	gcc -Wall -o 20151550.out p1.o p2.o p3.o
	rm *.o


p3.o : 20151550.h p3.c
	gcc -Wall -c -o p3.o p3.c

p2.o : 20151550.h p2.c
	gcc -Wall -c -o p2.o p2.c


p1.o : 20151550.h 20151550.c
	gcc -Wall -c -o p1.o 20151550.c

clean:
	rm 20151550.out
