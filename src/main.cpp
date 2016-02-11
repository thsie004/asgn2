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

    return 0;
}
