#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>


sem_t smutex;
sem_t kiseonikCekanje;
sem_t vodonikCekanje;


int kiseonik=0,vodonik=0;

pthread_t kiseonikNit,vodonikNit1,vodonikNit2;

int brojSpremnih = 0;

int veza(void){

   brojSpremnih++;
   if((brojSpremnih%3)==0)
       printf("Kreiran je novi molekul! I to je molekul broj %d\n\n",brojSpremnih/3);
   sleep(2);
   return 0;
}

void* kiseonikFja (void* arg){

   while(1){
       sem_wait(&smutex);
	
       kiseonik+=1;

	
       if(vodonik>=2){
           sem_post(&vodonikCekanje);
           sem_post(&vodonikCekanje);
           vodonik-=2;
           sem_post(&kiseonikCekanje);
           kiseonik-=1;
       }
       else {
           sem_post(&smutex);
       }

       sem_wait(&kiseonikCekanje);
       printf("Kiseonik je povezan\n");
       veza();

       sem_post(&smutex);
   }
}



void* vodonikFja(void* arg){
   while(1){

       sem_wait(&smutex);

       vodonik+=1;

       if(vodonik>=2 && kiseonik>=1){
           sem_post(&vodonikCekanje);
           sem_post(&vodonikCekanje);
           vodonik-=2;
           sem_post(&kiseonikCekanje);
           kiseonik-=1;
       }
       else{
           sem_post(&smutex);
       }



       sem_wait(&vodonikCekanje);

       printf("Vodonik je povezan\n");
       veza();
      
       }
}


main(){


   sem_init(&smutex,0,1);
   sem_init(&kiseonikCekanje,0,0);
   sem_init(&vodonikCekanje,0,0);

   pthread_create(&kiseonikNit,0,kiseonikFja, NULL);
   pthread_create(&vodonikNit1,0,vodonikFja, NULL);
   pthread_create(&vodonikNit2,0,vodonikFja, NULL);

   for(;;);

}


