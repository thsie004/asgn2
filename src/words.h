#ifndef _WORDS_H_
#define _WORDS_H_
#include <vector>
#include <string>

using namespace std;

/*This file and its implementations interprets and runs user input
 */


//Nodes can be either a bash command or a connector
class Node {
    protected:
        Node* next;
        string content;

    public:
        Node(string);

        Node* getNext();
        string getContent();
        void setNext(Node*);
        void run(int);
};

//Basically the head of the user input linked list. Organizes input into a 
//easily executable chain of Nodes.
class Line {
    protected:
        Node* head;

    public:
        Line(const vector<string> &);
        ~Line();

        void run();
        void printLine();
};

//Wangho: This function executes commands: cmd[0] should contain the
//        main command while anything else is flags, ***NEED a NULL at the
//        end of it.
int execute(char**);

#endif

