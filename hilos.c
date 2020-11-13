# include <sys/time.h>
# define __USE_GNU /* required before including  resource.h */
# include <sys/resource.h>
# include <time.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>

//https://www.tutorialspoint.com/c_standard_library/c_function_clock.htm


struct timeval t0 , t1 ; double delta = 0.0;//gettimeofday
clock_t start_t, end_t; double total_t; //clock
struct rusage usage; struct timeval start, end; double total_usage =0.0; //getrusage

void *saludo(void *arg){
	char *mensaje="Hello World";
	printf("%i. %s\n", *((int *)arg), mensaje);
	/*gettimeofday(&t1 , NULL );
	unsigned int t_final = t1.tv_sec *1000000+ t1.tv_usec ;
	unsigned int t_inicial = t0.tv_sec *1000000+ t0.tv_usec ;
	delta += ( t_final - t_inicial );*/
	/*end_t = clock();
	total_t += (double)(end_t - start_t);*/
	getrusage(RUSAGE_THREAD , &usage);
  	end = usage.ru_utime;
  	unsigned int tu_final = end.tv_sec*1000 + end.tv_usec/1000;
  	unsigned int tu_inicial = start.tv_sec*1000 + start.tv_usec/1000;
  	total_usage += (double)(tu_final-tu_inicial) ;
}

int main(int argc, char *argv[])
{
	int repeticion;
	pthread_t t_repeticion;

	if(argc!=2){
		printf("¡Valor de repetición no ingresado\n");
		return -1;
	}

	//printf("%s\n", argv[1]);

	repeticion = atoi(argv[1]);

	if(repeticion<0){
		printf("¡Número menor a 0 Threads por crear!\n");
		return -1;
	}if(repeticion==0){
		printf("¡Cero Threads creados -> Cero ms como resultado!\n");
		return -1;
	}

	for (int i = 0; i < repeticion; ++i){
		//gettimeofday(&t0 , NULL );
		/*start_t = clock();*/
		getrusage(RUSAGE_THREAD , &usage);
  		start = usage.ru_utime;
		pthread_create(&t_repeticion, NULL, saludo, &i);
		pthread_join(t_repeticion, NULL);
	}

	//printf ("Tiempo promedio en ms usando gettimeofday(): %f\n" , ( delta /repeticion)/1000 ) ; 
   	//printf("Tiempo promedio en ms usando clock(): %f\n", ((total_t/repeticion)/1000)  );
	/*printf("Started at: %ld.%ldms\n", start.tv_sec*1000, start.tv_usec/1000);
  	printf("Ended at: %ld.%ldms\n", end.tv_sec*1000, end.tv_usec/1000);*/
   	//printf("Tiempo promedio en ms usando getrusage(): %f\n", ((total_usage/repeticion))  );
	return 0;
}
