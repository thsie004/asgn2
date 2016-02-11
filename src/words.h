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

//Command has string in the form of e.g. "rm -r -f"
class Command: public Words {
    protected:
        string command;

    public:
        Command(Words*, string);
        ~Command();

        void run(int);
};

//Link will determine what value to pass onto the next command
//depending on what value it has received.
class Link: public Words {
    protected:
        string connector;

    public:
        Link(Words*, string);
        ~Link();

        void run(int);
};

//Basically the head of the user input. Organizes input into a 
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

