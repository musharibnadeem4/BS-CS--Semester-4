//Musharib
//i201764
#include <iostream>
#include <string>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>

using namespace std;    
int main() {
    int i;
    void* shared_memory;
    char buff[100];
    int shmid;

    shmid = shmget(static_cast<key_t>(2345), 1024, 0666 | IPC_CREAT);
    shared_memory = shmat(shmid, nullptr, 0);
    cout << "Process attached at " << shared_memory << endl;
    cout << "Enter some data to write to shared memory: ";
    cin.getline(buff, 100);
    strcpy(static_cast<char*>(shared_memory), buff);
    cout << "You wrote: " << static_cast<char*>(shared_memory) << endl;
}
