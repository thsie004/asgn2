#ifndef _WORDS_H
#define _WORDS_H
#include <vector>
#include <string>

using namespace std;

//Base class
class Words {
    protected:
        Words* L;
        Words* R;

    public:
        Words(){};
        virtual int execute(int) = 0;
};

//Command has strings in the form of e.g. "rm -r -f"
class Command: public Words {
    protected:
        string command;

    public:
        Command(Words*, Words*, string);
        ~Command();

        int execute(int);
};

//Link will determine what value to pass onto the next command
//depending on what value it has received.
class Link: public Words {
    protected:
        string connector;

    public:
        Link(Words*, Words*, string);
        ~Link();

        int execute(int);
};

//Basically the head of the user input. Organizes input into a 
//easily executable chain of Words.
class Line {
    protected:
        Words* head;

    public:
        Line(vector<string>);
        ~Line();
        void run();
};

//Function that converts strings into array of cstring for execvp.
char** gimmeArgs(string);

#endif

