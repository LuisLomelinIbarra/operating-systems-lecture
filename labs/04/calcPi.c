/*	Program: Calculate Pi Via threads
 *
 *	Description: The Idea of this code is to calculate the value of pi
 *	using threads
 *
 *	By: Luis Fernando Lomelín Ibarra A01177015
 * */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TH_NUM 1000 //the number of threads to use
#define POINTNUM 100000000 //The number of points to use

pthread_mutex_t mutsum;// A global mutex variable to get the sum

//the total sum of points in
int sumP;
//Function to calculate if the point is in the circle
double calDist(double x,double y){
	return sqrt( pow(x,2) + pow(y,2) );
}

/*
 * Routine: calculatePoints
 *
 * Desc:
 * 	It generates a an amount of random points specified by it's argument
 * 	and gives as a return value the number of points that are inside of the
 * 	circle.
 * */



void *calculatePoints(void *nop){
	double x,y,r;
	int pointsin=0;
	r=rand();
	int pin=0;
	int n= (int) nop;
	for(int i=0; i<n;i++){
		x=rand();
		y=rand();
		if(r<=calDist(x,y) ) pin++;
	}
	//printf("Hello I calculated: %d\n",pin);
	pthread_mutex_lock(&mutsum);
	sumP+=pin;
	pthread_mutex_unlock(&mutsum);
	pthread_exit((void*) 0);


}


int main(int argc,char *argv[]){
	pthread_t threads[TH_NUM]; //Setting the thread array
	pthread_attr_t attr;
	void* status;
	int nop = POINTNUM/TH_NUM;
	//printf("%d\n",nop);
	int pin=0;
	sumP=0;
	srand(time(0));


	//Starting Mutex
	pthread_mutex_init(&mutsum,NULL);

	//Setting the threads as joinable,
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	//Calling all of the threads to work
	
	for(int i=0;i<TH_NUM;i++){
		  pthread_create(&threads[i],&attr,calculatePoints,(void *)nop);
	}

	pthread_attr_destroy(&attr);
	
	for(int i=0; i<TH_NUM;i++){
		pthread_join(threads[i],&status);
	}
	//pin=sumP;
	//printf("\nPin: %d\n",sumP);
	//Calculate Pi
	double pi= 4.0* sumP/(POINTNUM*1.0); 

	//Print Results
	printf("The value of Pi calculated by: \n- %d points\n- %d threads\nIs Pi = %f \n",POINTNUM,TH_NUM,pi);
	pthread_mutex_destroy(&mutsum);
	pthread_exit(NULL);
	return 0;
}
