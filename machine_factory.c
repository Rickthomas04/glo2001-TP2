#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_MACHINES 5
#define ARTICLES_PER_ROUND 100
#define NUM_ROUNDS 3

pthread_mutex_t mutex;
pthread_barrier_t barrier;
int stock[ARTICLES_PER_ROUND];
int current_index;

void *travail_machine(void *arg) {
	int id = *(int *)arg;
    	for (int round = 0; round < NUM_ROUNDS; round++) { 
        	pthread_barrier_wait(&barrier); //on synchronise les threads
        	if (id == 0) {
            		printf("===== Début du round %d =====\n", round);
        		}
        	pthread_barrier_wait(&barrier); 
        	while (1) { 			//boucle pour parcourir les articles
            		pthread_mutex_lock(&mutex); //prend le mutex pour effectuer le travail
            		if (current_index >= ARTICLES_PER_ROUND) {
                		pthread_mutex_unlock(&mutex); //libere le mutex si boucle fini
               	break;
            		}
           		int article = stock[current_index];	//effectue le travail
            		int index = current_index;
            		current_index++;
            		pthread_mutex_unlock(&mutex);
			int resultat = article * 2; 
            		printf("Machine %d (round %d) : Traitement de l'article %d -> Résultat %d\n", id, round, index + round * ARTICLES_PER_ROUND, resultat);
            		usleep(10000);
        		}
        	pthread_barrier_wait(&barrier); //synchronise les threads apres la boucle
        	if (id == 0) {
            		printf("===== Fin du round %d =====\n", round);
        	}
        	pthread_barrier_wait(&barrier);
        	if (id == 0) {
            		for (int i = 0; i < ARTICLES_PER_ROUND; i++) {
                		stock[i] = i + round * ARTICLES_PER_ROUND; //maj du stock
            		}
            		current_index = 0;
        	}
        	pthread_barrier_wait(&barrier);
    	}

    	return NULL;

}
 

int main(){
 
	printf("Bienvenue dans l'usine de production !\n");
	printf("Lancement de %d machines pour traiter %d articles sur %d rounds.\n", NUM_MACHINES, ARTICLES_PER_ROUND, NUM_ROUNDS);
 
	pthread_t machines[NUM_MACHINES]; //initialise tableau et variables
	int ids[NUM_MACHINES];
	current_index = 0;
	pthread_mutex_init(&mutex, NULL); //cree mutex et barriere
	pthread_barrier_init(&barrier, NULL, NUM_MACHINES);
	for (int i = 0; i < ARTICLES_PER_ROUND; i++) {
        stock[i] = i; //remplit tableau d'articles
    	}
    	for (int i = 0; i < NUM_MACHINES; i++) {
        ids[i] = i;
        pthread_create(&machines[i], NULL, travail_machine, &ids[i]); //cree les threads
    	}
    	for (int i = 0; i < NUM_MACHINES; i++) {
        pthread_join(machines[i], NULL); //attendre la fin de chaque thread
    	}
    	pthread_mutex_destroy(&mutex); //detruire mutex et barriere
    	pthread_barrier_destroy(&barrier);
    	printf("Tous les rounds sont terminés. Fin de l'usine de production.\n");
 
}

