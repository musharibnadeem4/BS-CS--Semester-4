/*Musharib
i201764*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define num_persons 5
#define max_capacity 10

sem_t water; 
sem_t personSem;

int reservoir = 0;

void* personThread(void* personID) {
  int person = *((int*)personID);

  while (1) {
    sem_wait(&personSem);

    if (reservoir >= 5) {
      reservoir -= 5;
      printf("Person %d took 5L of water. Current water level: %dL\n", person, reservoir);
    } else {
      printf("Person %d waiting for water. Current water level: %dL\n", person, reservoir);
    }

    sem_post(&personSem);

    int sleepTime = rand() % 3 + 1;
    sleep(sleepTime);
  }

  return NULL;
}

void* filterThread(void* arg) {
  while (1) {
    sem_wait(&water);

    if (reservoir < max_capacity) {
      reservoir += 10;
      printf("Filter added 10L of water. Current water level: %dL\n", reservoir);
    }

    sem_post(&water);

    sleep(2);
  }

  return NULL;
}

int main() {
  pthread_t filter, persons[num_persons];
  int personIDs[num_persons];

  sem_init(&water, 0, 1);
  sem_init(&personSem, 0, 3);

  for (int i = 0; i < num_persons; i++) {
    personIDs[i] = i + 1;
    pthread_create(&persons[i], NULL, personThread, &personIDs[i]);
  }

  pthread_create(&filter, NULL, filterThread, NULL);

  for (int i = 0; i < num_persons; i++) {
    pthread_join(persons[i], NULL);
  }
  pthread_join(filter, NULL);

  sem_destroy(&water);
  sem_destroy(&personSem);

  return 0;
}
