# include <sys/types.h>
# include <sys/time.h>
#include <sys/wait.h>
# include <time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
#include <getopt.h>

bool cflag = false;
bool gflag = false;
bool rflag = false;

int main (int argc, char *argv[]) {
	struct timeval t0 , t1 ; //gettimeofday
	clock_t start_t, end_t; double total_t; //clock
	int repeticion, opt;
	pthread_t t_repeticion;


	/*if(argc!=3){
		printf("¡Valor de repetición no ingresado\n");
		return -1;
	}*/

	while( (opt= getopt(argc, argv, "c:g:r:h")) != -1 ){
		switch(opt){
			case "c":
				cflag = true;
				repeticion = (int) optarg;
				break;
			case "g":
				gflag = true;
				repeticion = (int) optarg;
				break;
			case "r":
				rflag = true;
				repeticion = (int) optarg;
				break;
			case "h"
			default:
				printf("Calcula el tiempo de ejecución de: \n");
				printf("gettimeofday>>\t ./fork -g <num_tareas> \n");
				printf("clock>>\t ./fork -c <num_tareas> \n");
				printf("getrusage>>\t ./fork -c <num_tareas> \n");
				return 0;

		}

	}

	//repeticion = atoi(argv[1]);

	if(repeticion<0){
		printf("¡Número menor a 0 procesos por crear!\n");
		return -1;
	}if(repeticion==0){
		printf("¡Cero procesos creados -> Cero ms como resultado!\n");
		return -1;
	}

	int id = -1;

	if (gflag){
		printf("---GETTIMEOFDAY---\n");
		gettimeofday (&t0 , NULL );
		for (int i = 0 ; i < repeticion ; i ++ ) {
			id = fork ();
			if ( id == 0)
				return 0; 		
		}	
		if ( id > 0) {
			wait(NULL);
			gettimeofday (&t1 , NULL );
			unsigned int t_final = t1.tv_sec *1000000+ t1.tv_usec ;
			unsigned int t_inicial = t0.tv_sec *1000000+ t0.tv_usec ;
			double delta = (t_final - t_inicial);
			printf ( "Tiempo promedio en ms usando gettimeofday():!%f\n" , ((( delta ) /repeticion)/1000) ) ;
		}

	}else if (cflag){
		printf("---CLOCK---\n");
		start_t = clock();
		for (int i = 0 ; i < repeticion ; i ++ ) {
			id = fork ();
			if ( id == 0)
				return 0; 			
		}
		if ( id > 0) {
			wait(NULL);
			end_t = clock();
			total_t = (double)(end_t - start_t) / 1000.0;
	   		printf("Tiempo promedio en ms usando clock(): %f\n", total_t  );
		}
		
	}else if(rflag){
		printf("GETRUSAGE\n");

	}

	//gettimeofday (&t0 , NULL ) ;
	start_t = clock();
	for (int i = 0 ; i < repeticion ; i ++ ) {
		id = fork ();
		if ( id == 0){
			//printf("Hola\n");
			return 0; 
		} 
	}
	if ( id > 0) {
		wait(NULL);
		//gettimeofday (&t1 , NULL );
		end_t = clock();
		total_t = (double)(end_t - start_t) / 1000.0;
   		printf("Total time taken by CPU: %f\n", total_t  );
		/*unsigned int t_final = t1.tv_sec *1000000+ t1.tv_usec ;
		unsigned int t_inicial = t0.tv_sec *1000000+ t0.tv_usec ;
		double delta = (t_final - t_inicial);
		printf ( "%f Milisegundos promedios para crear procesos!\n" , ((( delta ) /repeticion)/1000) ) ; */
	}
	//return 0;
}
