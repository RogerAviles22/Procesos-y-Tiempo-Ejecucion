# include <sys/types.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <sys/resource.h>
# include <time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

bool cflag = false;
bool gflag = false;
bool rflag = false;

int main (int argc, char **argv) {
	struct timeval t0 , t1 ; //gettimeofday
	clock_t start_t, end_t; double total_t; //clock
	struct rusage usage; struct timeval ru_start, ru_end;//getrusage
	int repeticion;
	char opt;

	while( (opt= getopt(argc, argv, "c:g:r:h")) != -1 ){
		switch(opt){
			case 'c':
				cflag = true;
				break;
			case 'g':
				gflag = true;
				break;
			case 'r':
				rflag = true;
				break;
			case 'h':
			default:
				printf("Calcula el tiempo de ejecución de las tareas con: \n");
				printf("gettimeofday>>\t ./fork -g <num_tareas> \n");
				printf("clock>>\t\t ./fork -c <num_tareas> \n");
				printf("getrusage>>\t ./fork -r <num_tareas> \n");
				return 0;

		}
	}

	printf("----PROCESOS");

	repeticion = atoi(argv[2]);

	if(repeticion<0){
		printf("¡Número menor a 0 procesos por crear!\n");
		return -1;
	}if(repeticion==0){
		printf("¡Cero procesos creados -> Cero ms como resultado!\n");
		return -1;
	}

	int id = -1;

	if (gflag){
		printf("\t\t\t---GETTIMEOFDAY---\n");
		gettimeofday (&t0 , NULL );
		for (int i = 0 ; i < repeticion ; i ++ ) {
			id = fork ();
			if ( id == 0)
				return 0; 		
		}	
		if ( id > 0) {
			wait(NULL);
			gettimeofday (&t1 , NULL );
			unsigned int t_final = t1.tv_sec *1000+ t1.tv_usec/1000 ;
			unsigned int t_inicial = t0.tv_sec *1000+ t0.tv_usec/1000 ;
			double delta = (t_final - t_inicial);
			printf ( "Tiempo promedio en ms usando gettimeofday(): %f\n" ,  delta /repeticion ) ;
		}

	}else if (cflag){
		printf("\t\t\t---CLOCK---\n");
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
	   		printf("Tiempo promedio en ms usando clock(): %f\n", total_t/repeticion  );
		}
		
	}else if(rflag){
		printf("\t\t\t---GETRUSAGE---\n");

		getrusage(RUSAGE_SELF, &usage);
  		ru_start = usage.ru_stime;
		for (int i = 0 ; i < repeticion ; i ++ ) {
			id = fork ();
			if ( id == 0)
				return 0; 			
		}
		if ( id > 0) {
			wait(NULL);
			getrusage(RUSAGE_SELF, &usage);
  			ru_end = usage.ru_stime;
			unsigned int t_final = ru_end.tv_sec *1000+ ru_end.tv_usec/1000 ;
			unsigned int t_inicial = ru_start.tv_sec *1000+ ru_start.tv_usec/1000 ;
			double delta = (t_final - t_inicial);

	   		printf("Tiempo promedio en ms usando getrusage(): %f\n", delta/repeticion  );
		}
	}
}
