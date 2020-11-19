# include <sys/time.h>
# define __USE_GNU /* required before including  resource.h */
# include <sys/resource.h>
# include <time.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

//https://www.tutorialspoint.com/c_standard_library/c_function_clock.htm
//https://stackoverflow.com/questions/10509660/getting-getrusage-to-measure-system-time-in-c
//https://linux.die.net/man/2/getrusage

struct timeval t0 , t1 ; double delta = 0.0;//gettimeofday
clock_t start_t, end_t; double total_t; //clock
struct rusage usage; struct timeval start, end; double total_usage =0.0; //getrusage

bool cflag = false;
bool gflag = false;
bool rflag = false;

void *saludo_gtod(void *arg);
void *saludo_clock(void *arg);
void *saludo_gru(void *arg);

int main(int argc, char *argv[]){

	int repeticion;
	pthread_t t_repeticion;
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
				printf("gettimeofday>>\t ./hilos -g <num_tareas> \n");
				printf("clock>>\t\t ./hilos -c <num_tareas> \n");
				printf("getrusage>>\t ./hilos -r <num_tareas> \n");
				return 0;
		}
	}

	printf("----HILOS");

	repeticion = atoi(argv[2]);

	if(repeticion<0){
		printf("¡Número menor a 0 Threads por crear!\n");
		return -1;
	}if(repeticion==0){
		printf("¡Cero Threads creados -> Cero ms como resultado!\n");
		return -1;
	}

	if(gflag){
		printf("\t\t\t---GETTIMEOFDAY---\n");
		for (int i = 0; i < repeticion; ++i){
			gettimeofday(&t0 , NULL );
			pthread_create(&t_repeticion, NULL, saludo_gtod, NULL);
			pthread_join(t_repeticion, NULL); 
		}
		printf ("Tiempo promedio en ms usando gettimeofday(): %f\n" , ( delta /repeticion)/1000 ) ; 

	}
	else if(cflag){
		printf("\t\t\t---CLOCK---\n");
		for (int i = 0; i < repeticion; ++i){		
			start_t = clock();
			pthread_create(&t_repeticion, NULL, saludo_clock, NULL);
			pthread_join(t_repeticion, NULL);
		}
		printf("Tiempo promedio en ms usando clock(): %f\n", ((total_t/repeticion)/1000)  );

	}else if(rflag){
		printf("\t\t\t---GETRUSAGE---\n");
		for (int i = 0; i < repeticion; ++i){
			getrusage(RUSAGE_SELF , &usage);
	  		start = usage.ru_stime;
			pthread_create(&t_repeticion, NULL, saludo_gru, NULL);
			pthread_join(t_repeticion, NULL);
		}
		/*printf("Started at: %ld.%ldms\n", start.tv_sec*1000, start.tv_usec/1000);
  		printf("Ended at: %ld.%ldms\n", end.tv_sec*1000, end.tv_usec/1000);*/
   		printf("Tiempo promedio en ms usando getrusage(): %f\n", ((total_usage/repeticion))  );
	}

	return 0;
}

void *saludo_gtod(void *arg){
	gettimeofday(&t1 , NULL );
	unsigned int t_final = t1.tv_sec *1000000+ t1.tv_usec ;
	unsigned int t_inicial = t0.tv_sec *1000000+ t0.tv_usec ;
	delta += ( t_final - t_inicial );	
	printf("\n");
}

void *saludo_clock(void *arg){
	end_t = clock();
	total_t += (double)(end_t - start_t);
}

void *saludo_gru(void *arg){
	getrusage(RUSAGE_SELF , &usage);
  	end = usage.ru_stime;
  	unsigned int tu_final = end.tv_sec*1000 + end.tv_usec/1000;
  	unsigned int tu_inicial = start.tv_sec*1000 + start.tv_usec/1000;
		/*printf("Started at: %ld.%ldms\n", start.tv_sec*1000, start.tv_usec/1000);
  		printf("Ended at: %ld.%ldms\n", end.tv_sec*1000, end.tv_usec/1000);*/
  	//printf("fin %d\n", tu_final);printf("init %d\n", tu_inicial);
  	total_usage += (double)(tu_final-tu_inicial) ;
  	//printf("total %f\n", total_usage);
}
