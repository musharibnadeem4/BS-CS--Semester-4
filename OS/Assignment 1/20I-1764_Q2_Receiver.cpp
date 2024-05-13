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
    shmid = shmget(static_cast<key_t>(2345), 1024, 0666);
    shared_memory = shmat(shmid, NULL, 0);
    cout << "Process attached at " << shared_memory << endl;
    cout << "Data read from shared memory is: " << static_cast<const char*>(shared_memory) << endl;
}
