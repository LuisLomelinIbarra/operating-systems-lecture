/*
 *	Program: Milk Factory
 *	Description: It is a C program that emulates a milk factory with 
 *	5 sellers that sell a random number of milk bottles. Since the
 *	Factory can only produce a number of bottles, the program must manage
 *	so that the sellers do not sell more than what the factory can produce
 *
 *	By: Luis Fernando Lomelín Ibarra
 *	ID: A01177015
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define factoryMax 1000
pthread_mutex_t lock;
#define consoleRed  "\033[31m"
#define consoleBlue  "\033[34m"
#define consoleGreen  "\033[32m"
#define consolePurpule  "\033[35m"
#define consoleGrey  "\033[37m"

#define consoleReset "\033[0m"


typedef struct{
	int consNum; //Number of customers that will be sold to
	unsigned int *seed; // Random seed to use
	int tid;
} thdata;

thdata tdata;

int milkSellers=5;
int costumerMax=5;
int factoryStock=factoryMax;
int sellCap=200;

void sellMilk(void * data){
	thdata *dat =  (thdata*)data;
	int sellMax=dat->consNum;
	int totalSold=0;
	int managedToSell=0;
	int remain=0;
	//int id= dat->tid;
	//printf("\nID: %d\n",id);
	printf(consoleGrey "\n+++++++++++++++++++++++++++++++\n A seller has started it's route\n" consoleReset);	
//	printf(consoleRed "\n;;;;;;;;;;;;;;;\n Hi there this thread sees the factory stat at the begining as, %d \n;;;;;;;;;;;;;;;;;;;;;;;;;;" consoleReset ,factoryStock);
	for(int i=0; i<sellMax;i++){
		//Decide a random number to sell milk
		int soldMilk=(int) (((double)rand_r((dat->seed))/RAND_MAX)*200+1);	
		printf(consoleGreen "\n\t Planned Qty. Sold in to customer %d is %d\n" consoleReset ,(i+1),soldMilk);
		//Critical Section; seeing if the amount to sell is enough
		pthread_mutex_lock(&lock);
		if(factoryStock-soldMilk>=0){
			factoryStock-=soldMilk;
			totalSold+=soldMilk;
		}else{
			remain = factoryStock-soldMilk;
			managedToSell = soldMilk+remain;
			totalSold+=managedToSell;
			factoryStock-=managedToSell;
			printf(consoleRed "\n---------------------------------------------\nThe factory does not have more milk; Of the order %d bottles; I could only sell %d bottles to Customer %d. \n\tSorry....." consoleReset,soldMilk,managedToSell,(i+1));
			i=sellMax;

		}
		pthread_mutex_unlock(&lock);
	}
	
	printf(consoleBlue "\n\n>> The seller Total milk sold: %d\n---------------------------\n\n" consoleReset,totalSold);
	pthread_exit((void*) totalSold);

}

int main(){
	pthread_mutex_init(&lock,NULL);
	pthread_t threads[milkSellers];
	int sellerSold[milkSellers];
	pthread_attr_t atrib;
	
	pthread_attr_init(&atrib);
	pthread_attr_setdetachstate(&atrib,PTHREAD_CREATE_JOINABLE);
	int getSold;
	tdata.consNum=costumerMax;
	tdata.tid=1;
	unsigned int tm = time(0);

	tdata.seed=&tm;
	for(int i=0; i< milkSellers;i++){
		
		//printf("%d",tdata.tid);
		pthread_create(&threads[i],&atrib,sellMilk,(void *) &tdata );
		*(tdata.seed)+=3;
		tdata.tid = (2+i);
		
	}
	
	pthread_attr_destroy(&atrib);


	for(int i=0; i<milkSellers;i++){
		pthread_join(threads[i],(void*) &getSold);
		sellerSold[i]=getSold;
	}
	
	printf(consolePurpule"#########################################################"consoleReset);
	printf("\n\n All Sellers have returned the factory Stock is now: %d\n\n\n",factoryStock);
	int sum=0;
	for(int i=0;i<milkSellers;i++){
		printf(consoleGrey "\n\tSeller %d sold %d bottles\n" consoleReset,(i+1),sellerSold[i]);
		sum+=sellerSold[i];
	}
	if(sum!=factoryMax){ 
		printf(consoleRed "Sold More than the factory can" consoleReset);
		
	}else{
		printf(consoleGreen "\nThe factory succesfully sold %d bottles of the daily %d bottles\n\n\n" consoleReset, sum, factoryMax);
	}
	pthread_mutex_destroy(&lock);
	pthread_exit(NULL);


	return 0;
}
