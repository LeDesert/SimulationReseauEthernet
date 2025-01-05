FLAGS=-Wall -Wextra

all: main

ip.o: ip.c ip.h
	gcc -c $(FLAGS) ip.c

mac.o: mac.c mac.h
	gcc -c $(FLAGS) mac.c

station.o: station.c station.h
	gcc -c $(FLAGS) station.c

switch.o: switch.c switch.h
	gcc -c $(FLAGS) switch.c

graphe.o: graphe.c graphe.h
	gcc -c $(FLAGS) graphe.c

trameEthernet.o : trameEthernet.c trameEthernet.h
	gcc -c $(FLAGS) trameEthernet.c

main.o: main.c
	gcc -c $(FLAGS) main.c

main: ip.o mac.o station.o switch.o graphe.o main.o trameEthernet.o
	gcc -o main main.o ip.o mac.o station.o switch.o graphe.o trameEthernet.o

clean:
	rm *.o main
