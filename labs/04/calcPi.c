/*	Program: Calculate Pi Via threads
 *
 *	Description: The Idea of this code is to calculate the value of pi
 *	using threads
 *
 *	By: Luis Fernando Lomelín Ibarra A01177015
 *	Link to Chart:shorturl.at/alDL4
 *
 *	Link to Repo: https://github.com/LuisLomelinIbarra/operating-systems-lecture
 * */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define POINTNUM 10000 //The number of points to use

pthread_mutex_t mutsum;// A global mutex variable to get the sum

//Clock Variables to take time
clock_t start,end;

// Important data to pass on to each thread
typedef struct{
	int nop; //Number of points to use in the calc
	unsigned int *seed; // Random seed to use
} thdata;

//Variable to carry info for the cal.
thdata tdat;

//the total sum of points in
int sumP;

//Function to calculate if the point is in the circle
double calDist(double x,double y){
	return sqrt( x*x + (y*y) );
}

/*
 * Routine: calculatePoints
 *
 * Desc:
 * 	It generates a an amount of random points specified by it's argument
 * 	and gives as a return value the number of points that are inside of the
 * 	circle.
 * */



void *calculatePoints(void *s){
	double x,y,r;
	thdata *st;
        st = (thdata *) s;
	int pin=0;
	int n= st->nop;
	for(int i=0; i<n;i++){
		x=rand_r((st->seed))/(double) RAND_MAX;
		y=rand_r((st->seed))/(double) RAND_MAX;
		if(calDist(x,y)< 1) pin++;
	}
	//printf("Hello I calculated: %d\n",pin);
	pthread_mutex_lock(&mutsum);
	sumP+=pin;
	pthread_mutex_unlock(&mutsum);
	//printf("Finished Mutex\n");
	pthread_exit((void*) 0);


}


int main(int argc,char *argv[]){
	
	double timeused; // the time in seconds that it took to do the operation
	int TH_NUM;
	TH_NUM =(argc<2) ? 1:atoi(argv[1]); // Number of threads
	
	//printf("%d",TH_NUM);
	pthread_t threads[TH_NUM]; //Setting the thread array
	pthread_attr_t attr;
	void* status;
	int nop = POINTNUM/TH_NUM;
	//printf("%d\n",nop);
	int pin=0;
	sumP=0;
	start=clock();
	tdat.nop=nop;
	unsigned int tm = time(0);	
	(tdat.seed)= &tm;

	//Starting Mutex
	pthread_mutex_init(&mutsum,NULL);

	//Setting the threads as joinable,
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	//Calling all of the threads to work
	
	for(int i=0;i<TH_NUM;i++){
		  pthread_create(&threads[i],&attr,calculatePoints,(void *)&tdat);
		  *(tdat.seed)+=1;
	}

	pthread_attr_destroy(&attr);
	
	for(int i=0; i<TH_NUM;i++){
		pthread_join(threads[i],&status);
	}
	//pin=sumP;
	//printf("\nPin: %d\n",sumP);
	//Calculate Pi
	double pi= 4.0* (double) sumP/(double)POINTNUM; 
	end=clock();
	timeused = ((double)(end-start))/CLOCKS_PER_SEC;
	//Print Results
	printf("The value of Pi calculated by: \n- %d points\n- %d threads\nIs Pi = %f \nTime in used: %f s.\n",POINTNUM,TH_NUM,pi,timeused);
	pthread_mutex_destroy(&mutsum);
	pthread_exit(NULL);
	return 0;
}
