all:
		gcc -c dds.c -lwiringPi -Wall -O2
		gcc -c controls.c -lwiringPi -Wall -O2
		gcc -c main.c -Wall -O2
		gcc -c trx.c -Wall -O2
		gcc dds.o controls.o main.o trx.o -lwiringPi -lpthread -lncurses -o dds -Wall -O2
clean:
		rm -f *.o
		rm dds
