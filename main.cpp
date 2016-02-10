#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

//TOM: This function removes extra spaces at the head and tail of input
//     and also removes comments followed by #.
void prepareInput(string &input) {
    size_t hasHash = input.find("#");

    if (hasHash != string::npos) {
        input.erase(hasHash);
    }

    while (true) {
        if (input.at(0) == ' ') {
            input.erase(0,1);
        }else {
            break;
        }
    }

    while (true) {
        if (input.at(input.size() - 1) == ' ') {
            input.erase(input.size() - 1);
        }else {
            break;
        }
    }

    return;
}

//TOM: This function returns the initial position of the first delimiter.
//     If no delimiter is found, returns 99999999.
int firstCut(vector<size_t> v) {
    unsigned int min = 99999999;

    for (unsigned int i = 0; i < v.size(); i++) {
        if ((v.at(i) >= 0) && (min > v.at(i))) min = v.at(i);
    }

    return min;
}

//TOM: Pass in an empty vector and get back a vector with tokens
//     (e.g. "ls -a" or "||" or "echo hello") in sequential order.
//     Note that the "exit" command is not handled here, because
//     handling it in the interpretation of these tokens makes
//     way much more sense.
void getInput(vector<string> &tokenz) {
    string input;
    //TOM: 0 for ';', 1 for "&&", 2 for "||"
    vector<size_t> spots(3);
    //TOM: cutPos is the first position to cut our input.
    unsigned int cutPos;
    //TOM: coin stores command substrings temporarily.
    string coin;
    
    cout << "$ ";
    getline(cin, input);
    
    prepareInput(input);

    if (input.size() == 0) return;
    
    //TOM: Asks for more input when an attempt ends with valid connector.
    //     If a connector follows immediately after ';', this loop
    //     will continue to take inputs, and this error will be detected
    //     in the loop where the input is turned into tokens.
    if (input.size() > 2) {
        while (input.substr(input.size() - 2) == "||" ||
               input.substr(input.size() - 2) == "&&") {
            string appendage;

            cout << "> ";
            getline(cin, appendage);
            
            if (appendage.size() == 0) continue;
    
            prepareInput(appendage);
            input += appendage;
        }
    }
    
    //TOM: Removes ending ';' since if an input ends in ';' then
    //     the ';' really has no purpose but complicated codes below.
    if (input.at(input.size() - 1) == ';') {
        input.erase(input.size() - 1);
    }
                            
    while (true) {
        prepareInput(input);
        spots.at(0) = input.find(";");
        spots.at(1) = input.find("&&");
        spots.at(2) = input.find("||");

        cutPos = firstCut(spots);
        
        //TOM: Connector at the front, syntax must be wrong.
        //     Note that this doesn't check for invalid characters at front.
        //     Also error message is printed manually, as I can't come up
        //     with an alternative method.
        if (cutPos == 0) {
            cout << "-rshell: syntax error with unexpected connector";
            tokenz.clear();
            return;
        }

        if (cutPos == 99999999) {
            tokenz.push_back(input);
            
            break;

        }else {
            coin = input.substr(0, cutPos);
            prepareInput(coin);
            tokenz.push_back(coin);

            if (input.at(cutPos) == ';') {
                tokenz.push_back(";");
                input.erase(0, cutPos + 1);

            }else if (input.at(cutPos) == '|'){
                tokenz.push_back("||");
                input.erase(0, cutPos + 2);

            }else if (input.at(cutPos) == '&'){
                tokenz.push_back("&&");
                input.erase(0, cutPos + 2);

            }           
        }
    }

    return;
}

//Wangho: This function is for executing command, cmd[0] should contain the
//        main command while anything else is flags, ***NEED a NULL at the
//        end of it.
int execute(char* cmd[]) {
    pid_t pid;
    int status;
    int output = 0;
    int fd[2];    //Wangho: This is a piepe that for passing variable
                  //        (this case: output) between parent and child
                  //        process
    
    pipe(fd);
    pid = fork();
    if (pid < 0) {
        perror("rshell");
    }else if (pid == 0) {
        //Wangho: Close the read function of pipe
        close(fd[0]);
        //Wangho: This is child process
        output = execvp(cmd[0], cmd);

        //Wangho: Will only be here if the execp() fail, so output fail
        write(fd[1], &output, sizeof(output));
        perror("rshell");
        exit(0);
    }else if (pid > 0) {
        //Wangho: Close the write function of pipe
        close(fd[1]);
        //Wangho: This is parent process
        waitpid(pid, &status, 0);
        
        //Wangho: When child process is fully returned
        if(WIFEXITED(status)){
            //Wangho: Take the output value, if execvp fail, it returns -1
            read(fd[0], &output, sizeof(output));
            return output;
        }
    }
}

int main() {
    vector<string> tokens;
    getInput(tokens);
    int output = 0;

    cout << "tokens content: " << endl;
    for (unsigned int i = 0; i < tokens.size(); i++) {
        cout << tokens[i] << " size " << tokens[i].size() << endl;
    }

/*Wangho: TEST AREA*/
    string ls = "ls";
    string dash = "-l";
    char* cmd[] = {(char*)ls.c_str(), (char*)dash.c_str(), NULL};

    string ls2 = "ls";
    string dash2 = "-a";
    char* cmd2[] = {(char*)ls2.c_str(), (char*)dash2.c_str(), NULL};

    string connect = ";";       

    if (connect == ";" /*Wangho: if connector is ";"*/) {
        execute(cmd);
        execute(cmd2);
    }else if (connect == "&&" /*Wangho: if connecter is "&&"*/) {
        output = execute(cmd);

        if (output == 0) {
            execute(cmd2);
        }
    }else if (connect == "||" /*Wangho: if connecter is "||"*/) {
        output = execute(cmd);

        if (output == -1) {
            execute(cmd2);
        }
    }
/*end TEST AREA*/
    return 0;
}









