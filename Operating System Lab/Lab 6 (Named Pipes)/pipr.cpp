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
    
    fd1 = open("pipe1", O_RDONLY);
    fd2 = open("pipe2", O_WRONLY);
    
    while (1) {
        char buf_send[100];
        char buf_receive[100];
        
        read(fd1, buf_send, sizeof(buf_send));
        cout << "Received: " << buf_send << endl;
        
        cout << "Enter a response (type 'exit' to quit): ";
        cin>>buf_receive;
        
        write(fd2, buf_receive, strlen(buf_receive) + 1);
        
        if (strcmp(buf_receive, "exit") == 0) {
            break;
        }
    }
    
    close(fd1);
    close(fd2);
  
    
    return 0;
}

