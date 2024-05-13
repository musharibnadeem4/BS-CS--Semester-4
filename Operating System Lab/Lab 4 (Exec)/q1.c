#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
     execl("/home/musharib/Lab4/test", "test.exe", NULL);
     //execlp("./test", "test.exe", NULL);
    //execv("/home/musharib/Lab4/test", argv);
    //execle("/home/musharib/Lab4/test", "test.exe", NULL, envp);
     //execvp("./test", argv);
    //execve("/home/musharib/Lab4/test", argv, envp);
    
    return 0;
}

