#ifndef _WORDS_H_
#define _WORDS_H_
#include <vector>
#include <string>

using namespace std;

//Nodes can be either a bash command or a connector
class Node {
    public:
        Node* next;
        string content;

    public:
        Node(string);

        void setNext(Node*);
        Node* getNext();
        void run(int);
};

//Basically the head of the user input linked list. Organizes input into a 
//easily executable chain of Words.
class Line {
    protected:
        Node* head;

    public:
        Line(const vector<string> &);
        ~Line();

        void run(int);
        void printLine();
};

//Wangho: This function executes commands: cmd[0] should contain the
//        main command while anything else is flags, ***NEED a NULL at the
//        end of it.
int execute(char**);

#endif

