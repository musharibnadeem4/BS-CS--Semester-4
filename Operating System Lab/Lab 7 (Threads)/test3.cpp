#include <pthread.h>
#include <iostream>
using namespace std;

    pthread_t tid[5];

    int count = 0;
    void * counter(void *){
        count ++;
        cout<<"Count "<<count<<endl;
        char * status = "Success";

        pthread_exit((void*)status);

    }


int main(){
    void * status;
    for(int i= 0; i<5; i++){  
        pthread_create (&tid[i], NULL, counter, NULL);
    pthread_join(tid[i],&status);
    cout<<(char * ) status;
}
pthread_exit(NULL);
}
