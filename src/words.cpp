#include <cstring>
#include <iostream>
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

void Node::setNext(Node* n) {
    this->next = n;
}

Node* Node::getNext() {
    return this->next;
}

string Node::getContent() {
    return this->content;
}

//state = 0 means success
//state = -1 means fail
void Node::run(int state) {
    //BEGIN: if the node is a connector
    if (content == "||") {
        if (state == 0) {
            next->run(-1);
        }else {
            next->run(0);
        }
        return;
    }
    
    if (content == "&&") {
        if (state == 0) {
            next->run(0);
        }else {
            next->run(-1);
        }
        return;
    }

    if (content == ";") {
        if (next == 0) {
        }else {
            next->run(0);
        }
        return;
    }
    //END: if the node is a connector

    //Then the node is a bash command
    if (state == 0){
        //strtok messes a WHOLE LOT with dynamic allocated arrays
        //to a point where properly freeing them isn't possible.
        char test[999];
        char cstr[999];
        char *pch1,*pch2, quit[] = "exit";
        int count = 0, i = 0;
        
        
        //we're cutting string up twice
        //first time is to know how many slots in cmd do we need
        //second time creates the actual char** cmd into execute()        
        strcpy(test, content.c_str());
        strcpy(cstr, content.c_str());
        
        pch1 = strtok(test, " ");
        while (pch1 != NULL) {
            count++;
            pch1 = strtok(NULL, " ");
        }
        
        //-Werror and -pedantic gave me a lot of trouble for this
        //so there may be bugs/unknown segfaults.
        char* cmd[] = {0};
        pch2 = strtok(cstr, " ");
        while (pch2 != NULL) {
            cmd[i] = pch2;
            i++;
            pch2 = strtok(NULL, " ");
        }
        
        //quit rshell if the command is "exit" followed by whatever
        if (!strcmp(cmd[0], quit)) exit(0);

        cmd[count] = NULL;
             
        int status = execute(cmd);

        if (next == 0) return;
        next->run(status);
        
        //delete[] test;
        //delete[] cstr;
         
    }else {
        if (next == 0) return;
        next->run(-1);
    }                                    
}

//implementation of Line
Line::Line(const vector<string> &input) {
    head = 0;

    if (!input.empty()) {
        Node *a;
        
        head = new Node(input.at(0));
        a = head;

        for (unsigned int i = 1; i < input.size(); i++) {
            a->setNext(new Node(input.at(i)));
            a = a->getNext();
        }
        
        a->setNext(0);
    }
}

Line::~Line() {
    Node* a = head;
    Node* x;

    while (a != 0) {
        x = a;
        a = a->getNext();

        delete x;
    }
}

void Line::run(){
    if (head != 0) head->run(0);
}

//for testing purposes only
void Line::printLine() {
    Node* a = head;
    while (a != 0) {
        cout << a->getContent() << endl;
        a = a->getNext();
    }
}



//Executes a bash command, returns 0 if successful, -1 if uneventful.
//Note execvp returns success if the command is valid but the flag is not.
int execute(char* cmd[]) {
    pid_t pid;
    int status;
    int output = 0;
    int fd[2];    //Wangho: This is a pipe made for passing variable
                  //        (this case: output) between parent and child
                  //        process.
    

    if (pipe(fd) == -1) {
        perror("Pipe error");
    }

    pid = fork();
    if (pid < 0) {
        perror("fork() function error");
    }else if (pid == 0) {
        //Wangho: Close the read function of pipe
        if (close(fd[0] == -1)) {
            perror("Pipe closing error [0]");
        }
        //Wangho: This is the child process
        output = execvp(cmd[0], cmd);

        //Wangho: Will only be here if execp() fail, which means output failed
        if (write(fd[1], &output, sizeof(output)) == -1) {
            perror("Pipe writing error");
        }

        //perror for execvp
        perror(cmd[0]);
        exit(1);
    }else if (pid > 0) {
        //Wangho: Close the write function of pipe
        if (close(fd[1])) {
            perror("Pipe closing error [1]");
        }
        //Wangho: This is the parent process
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid error");
        }
        
        //Wangho: When child process is fully returned
        if(WIFEXITED(status)){
            //Wangho: Take the output value; if execvp failed, it returns -1
            if (read(fd[0], &output, sizeof(output)) == -1) {
                perror("Pipe reading error");
            }
            return output;
        }
    }
    return -1;
}


/*
int main(){
    vector<string> ss;
    ss.push_back("ls      -a");
    ss.push_back("&&");
    ss.push_back("echo why");
    ss.push_back(";");
    ss.push_back("echo hello");

    Line abc(ss);
    abc.run();
    return 0;
}
*/
