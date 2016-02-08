#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

//TOM: This function removes extra spaces at the head and tail of input
//and also removes comments followed by #.
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
//If no delimiter is found, returns 0.
int firstCut(vector<size_t> v) {
    unsigned int min = 99999999;

    for (unsigned int i = 0; i < v.size(); i++) {
        if ((v.at(i) > 0) && (min > v.at(i))) min = v.at(i);
    }

    if (min == 99999999) min = 0;

    return min;
}

int main() {
    string input;
    vector<string> tokenz;
    
    cout << " $ ";
    getline(cin, input);

    prepareInput(input);
 
    //TOM: 0 for ';', 1 for "&&", 2 for "||"
    vector<size_t> spots(3);
    //TOM: cutPos is the first position to cut our input.   
    unsigned int cutPos;
    //TOM: coin stores command substrings temporarily. 
    string coin;
    
    //TODO: This parser isn't checking for delimiters at the beginning of input yet.
    //      An error should be printed if that's the case
    //      Also delimiters at the end will need to be handled, too.

    while (true) {
        spots.at(0) = input.find(";");
        spots.at(1) = input.find("&&");
        spots.at(2) = input.find("||");
        
        cutPos = firstCut(spots);

        if (cutPos == 0) {
            prepareInput(input);
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
    
    cout << "Tokenz content: " << endl;
    for (unsigned int i = 0; i < tokenz.size(); i++) {
        cout << tokenz[i] << " size " << tokenz[i].size() << endl;
    }

    return 0;
}











