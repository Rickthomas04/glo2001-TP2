#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define CAPACITE_PARKING 5
#define NUM_VOITURES 10

pthread_mutex_t mutex;
int parked_cars = 0;
pthread_cond_t condition;

void Arrive(int voiture_id){
	printf("Voiture %d : Arrivée au parking.\n", voiture_id);//La voiture arrive
}

void Stationne(int voiture_id){
	pthread_mutex_lock(&mutex);
	while (parked_cars == CAPACITE_PARKING){ //on attend si parking est plein
	printf("Voiture %d : Parking plein, en attente...\n", voiture_id);
		pthread_cond_wait(&condition, &mutex);
	}
	parked_cars++; // la voiture se stationne, on augmente le conteur

	printf("Voiture %d : Se gare. Voitures garées actuellement : %d\n", voiture_id, parked_cars);
	pthread_mutex_unlock(&mutex);
	usleep((rand()+250000)%500000); //simule délai de stationnement aléatoire
}

void Depart(int voiture_id){
	pthread_mutex_lock(&mutex);
	parked_cars--; // la voiture sort du stationnement
	printf("Voiture %d : Quitte le parking. Voitures garées actuellement : %d\n", voiture_id, parked_cars);
	pthread_cond_signal(&condition); // on indique qu'une place est libre
	pthread_mutex_unlock(&mutex);
	
}

void *Thread_voiture(void *arg) {
	int id = *(int *)arg;
	usleep((rand()+250000)%500000);// simule temps d'arrivé aléatoire
	Arrive(id);
	Stationne(id);
	Depart(id);	
}
 

int main(){
 	srand(time(NULL));
	pthread_t voitures[NUM_VOITURES]; //initialise tableau et variables
	pthread_mutex_init(&mutex, NULL); //cree mutex et condition
	pthread_cond_init(&condition, NULL);
	int ids[NUM_VOITURES];
    	for (int i = 0; i < NUM_VOITURES; i++) {
        ids[i] = i + 1;
        pthread_create(&voitures[i], NULL, Thread_voiture, &ids[i]); //crée les threads
    	}
    	for (int i = 0; i < NUM_VOITURES; i++) {
        pthread_join(voitures[i], NULL); //attendre la fin de chaque thread
    	}
    	printf("Simulation terminée : toutes les voitures ont été traitées.\n");
    	pthread_mutex_destroy(&mutex); //détruire mutex et condition
    	pthread_cond_destroy(&condition);

}
