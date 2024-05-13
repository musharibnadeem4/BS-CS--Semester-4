//Musharib
//i20-1764
#include <iostream>
#include<unistd.h>
#include<pthread.h>

using namespace std;

int balance =0;
pthread_mutex_t mutex;

void *withdraw(void *tid){
    for(int i=0; i<10; i++){
        pthread_mutex_lock(&mutex);
        int readbalance = balance;
        printf("At time %d, the balance for withdrawal thread %d is %d\n", i, *(int*)tid, balance);
        if (readbalance < 10) {
            printf("Insufficient funds for withdrawal thread %d\n", *(int*)tid);
            pthread_mutex_unlock(&mutex);
            sleep(1);
            continue;
        } else {
            readbalance -= 10;
            sleep(1);
            balance = readbalance;
            printf("At time %d, the balance after withdrawal thread %d is %d\n", i, *(int*)tid, balance);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}
void *deposit(void *tid) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        int readbalance = balance;
        printf("At time %d, the balance before depositing thread %d is %d\n", i, *(int*)tid, balance);
        readbalance += 11;
        sleep(10);
        balance = readbalance;
        printf("At time %d, the balance after depositing thread %d is %d\n", i, *(int*)tid, balance);
        pthread_mutex_unlock(&mutex);
        sleep(10);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[4];
    int thread_ids[4] = {1, 2, 3, 4};
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, withdraw, (void *)&thread_ids[i]);
    }

    for (int i = 2; i < 4; i++) {
        pthread_create(&threads[i], NULL, deposit, (void *)&thread_ids[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
