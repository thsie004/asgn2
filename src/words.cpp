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
        char* test = new char[content.size()+1];
        char* cstr = new char[content.size()+1];
        char *pch1, *pch2;
        int count = 0, i = 0;

        strcpy(test, content.c_str());
        strcpy(cstr, content.c_str());
        pch1 = strtok(test, " ");
        while (pch1 != NULL) {
            count++;
            pch1 = strtok(NULL, " ");
        }

        char* cmd[count+1];
        pch2 = strtok(cstr, " ");
        while (pch2 != NULL) {
            cmd[i] = pch2;
            i++;
            pch2 = strtok(NULL, " ");
        }
        cmd[count] = NULL;
         
        int status = execute(cmd);

        if (next == 0) return;
        next->run(status);
        
        delete[] test;
        delete[] cstr;
    }else {
        if (next == 0) return;
        next->run(-1);
    }                                    
}

//implementation of Line
Line::Line(const vector<string> &input) {
    if (input.empty()) return;

    Node *a;

    head = new Node(input.at(0));
    a = head;

    for (int i = 1; i < input.size(); i++) {
        a->setNext(new Node(input.at(i)));
        a = a->getNext();
    }

    a->setNext(0);
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
    head->run(0);
}

void Line::printLine() {
    Node* a = head;
    while (a != 0) {
        cout << a->getContent() << endl;
        a = a->getNext();
    }
}



//Executes a bash command, returns 
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
        perror("Invalid command");
    }else if (pid > 0) {
        //Wangho: Close the write function of pipe
        if (close(fd[1])) {
            perror("Pipe closing error [1]");
        }
        //Wangho: This is the parent process
        waitpid(pid, &status, 0);
        
        //Wangho: When child process is fully returned
        if(WIFEXITED(status)){
            //Wangho: Take the output value; if execvp fail, it returns -1
            if (read(fd[0], &output, sizeof(output)) == -1) {
                perror("Pipe reading error");
            }
            return output;
        }
    }
}

int main(){
    vector<string> ss;
    ss.push_back("ls      -a");
    ss.push_back("&&");
    ss.push_back("pw");
    ss.push_back("||");
    ss.push_back("echo hello world");

    Line abc(ss);
    abc.run();

    return 0;
}

