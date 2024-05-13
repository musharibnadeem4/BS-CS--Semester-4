//Musharib 
//i201764
#include <iostream>
#include <pthread.h>

using namespace std;
#define SIZE 100
pthread_t tid[4];
int array[100];
int number;
bool found = false;
bool exitFlag = false;
void *find_number(void *arg)
{
    int *thread_number = (int *)arg;
    int start = *thread_number * (SIZE / 4);
    int end = start + (SIZE / 4);

    for (int i = start; i < end; i++)
    {
        if (found)
        {
            pthread_exit(NULL);
        }
        if (array[i] == number)
        {
            cout << "Thread Number: " << *thread_number + 1 << "\n";
            cout << "Number found at index " << i << endl;
            found = true;
            exitFlag = true;

            pthread_exit(NULL);
        }
        if (exitFlag)
        {
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}
int main()
{
    int thread_number[4];
    for (int i = 0; i < 4; i++)
    {
        thread_number[i] = i;
    }
    for (int i = 0; i < 100; i++)
    {
        array[i] = i + 1;
    }
    cout << "Enter a number you want to find: ";
    cin >> number;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    for (int i = 0; i < 4; i++)
    {
        pthread_create(&tid[i], &attr, find_number, (void *)&thread_number[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        pthread_join(tid[i], NULL);
    }
    pthread_exit(NULL);
    return 0;
}
