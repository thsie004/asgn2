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

void Line::run(int state){
    head->run(1);
}

void Line::printLine() {
    Node* a = head;
    while (a != 0) {
        cout << a->getContent() << endl;
        a = a->getNext();
    }
}



//Executes a bash command
int execute(char* cmd[]) {
    pid_t pid;
    int status;
    int output = 0;
    int fd[2];    //Wangho: This is a pipe made for passing variable
                  //        (this case: output) between parent and child
                  //        process.
    

    if (pipe(fd) == -1) {
        perror("Pipe create error");
    }

    pid = fork();
    if (pid < 0) {
        perror("fork() function error");
    }else if (pid == 0) {
        //Wangho: Close the read function of pipe
        if (close(fd[0] == -1)) {
            perror("Close pipe error [0]");
        }
        //Wangho: This is the child process
        output = execvp(cmd[0], cmd);

        //Wangho: Will only be here if execp() fail, which means output failed
        if (write(fd[1], &output, sizeof(output)) == -1) {
            perror("Pipe write error");
        }
        perror("Invalid command");
        exit(0);
    }else if (pid > 0) {
        //Wangho: Close the write function of pipe
        if (close(fd[1])) {
            perror("Close pipe error [1]");
        }
        //Wangho: This is the parent process
        waitpid(pid, &status, 0);
        
        //Wangho: When child process is fully returned
        if(WIFEXITED(status)){
            //Wangho: Take the output value; if execvp fail, it returns -1
            if (read(fd[0], &output, sizeof(output)) == -1) {
                perror("Pipe read error");
            }
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

    vector<string> ss;
    ss.push_back("hi");
    ss.push_back("||");
    ss.push_back("1");
    ss.push_back("&&");
    ss.push_back("2");
    ss.push_back("fdjsal;k");
    ss.push_back("fd");

    Line abc(ss);
    abc.printLine();


    return 0;
}

