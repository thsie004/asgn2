#include <cstdlib>
#include <iostream>
#include "input.cpp"
#include "words.cpp"

using namespace std;

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
