#ifndef _WORDS_H_
#define _WORDS_H_
#include <vector>
#include <string>

using namespace std;

//Base class
class Words {
    protected:
        Words* next;

    public:
        Words() {};
        virtual void run(int) = 0;
};

//Nodes can be either a bash command or a connector
class Node: public Words {
    protected:
        string content;

    public:
        Node(string);

        void setNext(Words*);
        void run(int);
};

//Basically the head of the user input linked list. Organizes input into a 
//easily executable chain of Words.
class Line: public Words {
    public:
        Line(vector<string>);
        ~Line();

        void run(int);
};

//Wangho: This function executes commands: cmd[0] should contain the
//        main command while anything else is flags, ***NEED a NULL at the
//        end of it.
int execute(char**);

#endif

