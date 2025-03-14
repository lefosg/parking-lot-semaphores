#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NO_SLOTS 10

pthread_t park_positions[NO_SLOTS];  //state of parking lot: 1=parked, 0=free

sem_t sem_park;
sem_t sem_unpark;

void* park();
void* unpark();
void print_parking_lot();

/**
 * Use of semaphores to simulate a parking lot with 10 positions available.
 * Each position is marked with 0 for free and 1 for parked.
 * To make it easier, one thread is not associated with its "own" car, it iterates the buffer, and 
 * if it finds a parked car, it's considered to be its own, so it unparks it. 
 */
int main(int argv, char** argc) {

    sem_init(&sem_park, 0, NO_SLOTS);
    sem_init(&sem_unpark, 0, 0);

    pthread_t tid_park[NO_SLOTS+1];  //one extra thread for testing
    pthread_t tid_unpark[NO_SLOTS];
    
    //create all threads
    pthread_create(&tid_park[10], NULL, &park, NULL);
    for (int i=0; i<NO_SLOTS; i++) {
        pthread_create(&tid_park[i], NULL, &park, NULL);
        pthread_create(&tid_unpark[i], NULL, &unpark, NULL);
    }
    
    for (int i=0; i<NO_SLOTS; i++) {
        pthread_join(tid_park[i], NULL);
        pthread_join(tid_unpark[i], NULL);
    }
    
    print_parking_lot();  //there should be one left

    return 0;
}

/**
 * One thread parks one car
 */
void* park() {
    sem_wait(&sem_park);
    
    for (int i =0; i<NO_SLOTS; i++) {
        if (park_positions[i] == 0) {
            park_positions[i] = 1;
            break;
        }
    }

    sem_post(&sem_unpark);
}

/**
 * One thread unparks one car
 */
void* unpark() {

    sleep(0.5); //sleep for a 0.5 ms just for a better simulation "wait for some cars to park"

    sem_post(&sem_unpark);

    for (int i =0; i<NO_SLOTS; i++) {
        if (park_positions[i] == 1) {
            park_positions[i] = 0;
            break;
        }
    }
    sem_post(&sem_park);
}

/**
 * For viewing the state of the parking lot
 */
void print_parking_lot() {
    printf("\n=== PARKING LOT START ===\n");
    for (int i =0; i<NO_SLOTS; i++) {
        printf("Position %d: %d\n",i,park_positions[i]);
    }
    printf("=== PARKING LOT END ===\n");
}