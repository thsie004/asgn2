#include <iostream>
#include <vector>
#include "header.h"

using namespace std;

int main()
{
        Line* line;
        string input;
        vector<string> v;

        while(cout << "$ ")
        {
                getline (cin, input);
                if(input != "exit")
                {
                        v.push_back(input);
                        line->memorize(input);               
                }
                else
                {
                        cout << endl;
                        break;
                }
        }
        
}
