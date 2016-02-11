#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "words.h"

//implementation of Node
Node::Node(string token) {
    this->content = token;
}

void Node::setNext(Words* n) {
    this->next = n;
}

void Node::run(int state) {
    //BEGIN: if the node is a connector
    if (content == "||") {
        if (state > 0) {
            next->run(0);
        }else {
            next->run(1);
        }
        return;
    }
    
    if (content == "&&") {
        if (state > 0) {
            next->run(1);
        }else {
            next->run(0);
        }
        return;
    }

    if (content == ";") {
        if (next == 0) {
        }else {
            next->run(1);
        }
        return;
    }
    //END: if the node is a connector

    //Then the node is a bash command
    if (state > 0){

    }else {
        if (next == 0) return;
        next->run(0);
    }                                    
}

//implementation of Line
Line::Line(vector<string> input) {
    if (input.empty()) return;

    this->next = new Node(input.at(1));

    Words *a, *b;

    for (int i = 0; i < input.size(); i++) {

    }

}

void Line::run(int state){
    next->run(1);
}





















int execute(char* cmd[]) {
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

