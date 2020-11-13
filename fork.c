# include <sys/types.h>
# include <sys/time.h>
#include <sys/wait.h>
# include <time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

int main (int argc, char *argv[]) {
	struct timeval t0 , t1 ; //gettimeofday
	clock_t start_t, end_t; double total_t; //clock
	int repeticion;
	pthread_t t_repeticion;

	if(argc!=2){
		printf("¡Valor de repetición no ingresado\n");
		return -1;
	}

	repeticion = atoi(argv[1]);

	if(repeticion<0){
		printf("¡Número menor a 0 procesos por crear!\n");
		return -1;
	}if(repeticion==0){
		printf("¡Cero procesos creados -> Cero ms como resultado!\n");
		return -1;
	}

	int id = -1;
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
