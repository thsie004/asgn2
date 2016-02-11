#include <cstdlib>
#include <limits.h>
#include <stdio.h>
#include "input.cpp"
#include "words.cpp"

using namespace std;

int main() {
    //Gets user name and host name
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];
    int result;

    result = gethostname(hostname, HOST_NAME_MAX);
    if(result){
        perror("gethostname");
        return 1;
    }

    result = getlogin_r(username, LOGIN_NAME_MAX);
    if(result){
       perror("getlogin_r");
       return 1;
    }
    //end of log in info

    vector<string> userInput;
    
    //loop for rshell
    while (true) {
        userInput.clear();
        getInput(userInput, username, hostname);
        Line runThis(userInput);
        runThis.run();
    }

    return 0;
}

