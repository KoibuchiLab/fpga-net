all:	
	gcc -fPIC -c -o sum.o sum.c
	gcc -fPIC -c -o sub.o sub.c
	gcc -shared -o libkmpi.so sum.o sub.o
	gcc -o test test.c libkmpi.so
	
