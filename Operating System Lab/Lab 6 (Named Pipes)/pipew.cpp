#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <error.h>

using namespace std;

int main() {
    int fd1, fd2;
    
    mkfifo("pipe1", 0666);
    mkfifo("pipe2", 0666);
    
    fd1= open("pipe1", O_WRONLY);
    fd2 = open("pipe2", O_RDONLY);
    
    while (1) {
        char buf_send[100];
        char buf_recieve[100];3
        
        cout << "Enter message to send (type 'exit' to quit): ";
        cin>>buf_send;
       
        write(fd1, buf_send, strlen(buf_send) + 1);
        
        read(fd2, buf_recieve, sizeof(buf_recieve));
        cout << "Received: " << buf_recieve << endl;
        
        if (strcmp(buf_send, "exit") == 0) {
            break;
        }
    }
    
    close(fd1);
    close(fd2);
   
    return 0;
}

