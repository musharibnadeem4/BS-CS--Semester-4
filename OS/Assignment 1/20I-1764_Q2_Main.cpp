//Musharib
//i201764
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main() {
    while (true) {
        int choice;
        cout << "Choose an option: "<<endl;
        cout << "1. Send message "<<endl;
        cout << "2. Receive message "<<endl;
        cout << "3. Exit" <<endl;
        cout << "Enter option: ";
        cin >> choice;
        if (choice == 3) {
            cout << "Exiting the program." << endl;
            break;
        }
        pid_t child_pid = fork();

        if (child_pid == 0) {
            // Child process
            if (choice == 1) {
                execl("q2s", "q2s", NULL);
            } else if (choice == 2) {
                execl("q2r", "q2r", NULL);
            } else {
                cerr << "Invalid option." << endl;
                exit(1);
            }
        } else if (child_pid < 0) {
            cerr << "Failed to fork a child process." << endl;
            exit(1);
        } else {
            int status;
            waitpid(child_pid, &status, 0);
        }
    }

    return 0;
}


//Synchonization and potential problems
/* 1.Race Conditions:
Multiple threads or processes using shared resources at the same time can damage data and produce unexpected outcomes.
To guarantee mutual exclusion, we can use semaphores or locks (mutexes), allowing only one thread or process to access the resource at a time.
2. Deadlocks
When two or more threads or processes wait endlessly for one another to release resources, a deadlock occurs and the system freezes.
To avoid or break out of deadlocks, use mechanisms for detecting or avoiding deadlocks, such as resource allocation graphs or timeouts.
3.Inconsistency in data:
Concurrent reading and writing of shared data by several threads or processes might result in inconsistent or corrupted data.
Use synchronisation techniques to prohibit concurrent writes and guarantee data consistency, such as read-write locks or transactions.*/

