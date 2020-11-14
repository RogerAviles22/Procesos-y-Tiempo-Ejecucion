all: fork hilos
		
fork: fork.c
	gcc fork.c -o fork

hilos: hilos.c
	gcc hilos.c -o hilos -l pthread 



