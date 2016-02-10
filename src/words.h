#ifndef WORDS_H
#define WORDS_H
#include <vector>
#include <string>

using namespace std;

class Words {
    public:
        Words(){};
        virtual void run() = 0;
};

class Command: public Words {
    protected:
        char* cmd[];

    public:
        Command(char* command[]) {
            strcpy(cmd, command);
        }
        char get_cmd(){return cmd;};
};

class Link: public Words{
    protected:
        Words* L;
        Words* R;

    public:
        Link(){};
        void run() = 0;
};

class Line: public Words{
    protected:
        vector<Words*> log;
        vector<string> comments;
        vector<string> line;

    public:
        Line(vector<string> input){
            line = input;
        }
        vector<string> evaluate(){return line;}
        void memorize(string);
};
















