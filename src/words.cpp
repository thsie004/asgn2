#include <cstring>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "words.h"
#include "test.cpp"

//implementation of Node
Node::Node(string token) {
    this->content = token;
}

Node::~Node() {
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
//state = 1 means success for the entire parentheses
void Node::run(int state, stack<int> &chart) {
    
    //BEGIN: if the node is a parenthesis
    if (content == "(") {
        chart.push(state);
        next->run(state, chart);
        return;
    }
    
    if (content == ")") {
        if (next == 0) return;
        
        if (chart.top() == 1) {
            chart.pop();
            next->run(0, chart);
            return;
        }else {
            chart.pop();
            next->run(state, chart);
            return;
        }
    }
    //END: if the node is a parenthesis
    
    //BEGIN: check the parentheses we're in
    if (chart.top() == -1) {
        next->run(-1, chart);
        return;
    }
    //END: check the parentheses we're in 
    
    //BEGIN: if the node is a connector
    if (content == "||") {
        if (state == 0) {
            next->run(-1, chart);
        }else {
            next->run(0, chart);
        }
        return;
    }
    
    if (content == "&&") {
        if (state == 0) {
            next->run(0, chart);
        }else {
            next->run(-1, chart);
        }
        return;
    }

    if (content == ";") {
        if (next == 0) {
        }else {
            next->run(0, chart);
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
        
        
        //we're cutting the string up twice
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
        //so a variable length array is no good.
        char* cmd[100];
        pch2 = strtok(cstr, " ");
        while (pch2 != NULL) {
            cmd[i] = pch2;
            i++;
            pch2 = strtok(NULL, " ");
        }
        
        //quit rshell if the command is "exit" followed by whatever
        if (!strcmp(cmd[0], quit)) {
            exit(0);
        }

        cmd[count] = NULL;
             
        int status = execute(cmd);

        //if there's no other nodes behind this then return
        if (next == 0) return;

        //successful command execution implies entire parentheses = true
        if (status == 0) {
            chart.pop();
            chart.push(1);
        }

        //all things checked, run next string
        next->run(status, chart);
        
    }else {
        if (next == 0) return;
        next->run(-1, chart);
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
    stack<int> chart;
    chart.push(0);
    if (head != 0) head->run(0, chart);
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
    int testResult;
    int output = 0;
    int fd[2];    //Wangho: This is a pipe made for passing variable
                  //        (this case: output) between parent and child
                  //        process.
    int last = 0;

    //Wangho: Getting the last non-null element's index
    for (int i = 0; cmd[i] != NULL; i++) {
        last++;
    }
    last = last - 1;
    //cout << "Last is: " << last << " content: " << cmd[last] << endl;

    //Wangho: Check if the command starts with "test"
    if (strcmp(cmd[0], "test") == 0 || (strcmp(cmd[0], "[") == 0 && strcmp(cmd[last], "]") == 0)) {
        //cout << "I'm in the test zone" << endl;
        if (last == 0) {
            return -1;
        }

        testResult = test(cmd);

        if (testResult == 0) {
            return 0;
        }else {
            return -1;
        }
    }else if ((strcmp(cmd[0], "[") == 0 && strcmp(cmd[last], "]") != 0)) {
        return -1;
    }

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
    ss.push_back("(");
    ss.push_back("echo hello");
    ss.push_back(")");
//    ss.push_back(";");
//    ss.push_back("echo hello");

    Line abc(ss);
    abc.run();
    return 0;
}
*/
