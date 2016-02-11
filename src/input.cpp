#ifndef _INPUT_CPP_
#define _INPUT_CPP_
#include <cstdlib>
#include <iostream>

using namespace std;

//TOM: This function removes extra spaces at the head and tail of input
//     and also removes comments followed by #.
void prepareInput(string &input) {
    size_t hasHash = input.find("#");
    
    if (hasHash != string::npos) {
        input.erase(hasHash);
    }
    
    while (true) {
        if (input.size() == 0) return;

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

//TOM: handles getting extra input if previous line ended in a connector
void getMoreInput(string &input) {
    string appendage;

    cout << "> ";
    getline(cin, appendage);
    prepareInput(appendage);

    if (appendage.size() == 0) {
        getMoreInput(input);
        return;
    }

    input += appendage;

    return;
}

//TOM: Pass in an empty vector and get back a vector with tokens
//     (e.g. "ls -a" or "||" or "echo hello") in sequential order.
//     Note that the "exit" command is not handled here, because
//     handling it in the interpretation of these tokens makes
//     way much more sense. Inputs like &&& or ||| will make the third
//     character an input instead of calling it syntax error.
//     Inputs like &&&& and |||| or more of the same connector
//     characters will invoke error detection.
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
            cout << "-rshell: syntax error with unexpected connector ";

            if (input.at(0) == '&') cout << "\"&&\"\n";
            if (input.at(0) == ';') cout << "\";\"\n";
            if (input.at(0) == '|') cout << "\"||\"\n";

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

                if (input.size() == 0) getMoreInput(input);

            }else if (input.at(cutPos) == '&'){
                tokenz.push_back("&&");
                input.erase(0, cutPos + 2);

                if (input.size() == 0) getMoreInput(input);

            }           
        }
    }

    return;
}

//IMPORTANT!: TOM: I have moved your execute to words.h and words.cpp because
//this function will only be used by that class, and not for inputs. Please update
//this function in those files when you start working.

#endif
