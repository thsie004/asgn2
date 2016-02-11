#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "words.h"

int execute(char** cmd) {
    pid_t pid;
    int status;
    int output = 0;
    int fd[2];    //Wangho: This is a pipe made for passing variable
                  //        (this case: output) between parent and child
                  //        process.
    
    pipe(fd);
    pid = fork();
    if (pid < 0) {
        perror("rshell");
    }else if (pid == 0) {
        //Wangho: Close the read function of pipe
        close(fd[0]);
        //Wangho: This is the child process
        output = execvp(cmd[0], cmd);

        //Wangho: Will only be here if execp() fail, which means output failed
        write(fd[1], &output, sizeof(output));
        perror("rshell");
        exit(0);
    }else if (pid > 0) {
        //Wangho: Close the write function of pipe
        close(fd[1]);
        //Wangho: This is the parent process
        waitpid(pid, &status, 0);
        
        //Wangho: When child process is fully returned
        if(WIFEXITED(status)){
            //Wangho: Take the output value; if execvp fail, it returns -1
            read(fd[0], &output, sizeof(output));
            return output;
        }
    }
}

/*
int main(){
    char a[] = "pwd";
    char b[] = "";
    char* cmd[3];
    cmd[0] = a;
    cmd[1] = b;
    cmd[2] = NULL;
    
    execute(cmd);
    return 0;
}
*/
