#ifndef WORDS_H
#define WORDS_H
#include <vector>
#include <string>

using namespace std;

class Words
{
        public:
                Words(){};
                virtual void run() = 0;
};

class Command: public Words
{
        protected:
                string cmd;
                vector<string> flags;

        public:
                Command(string command)
                {
                        cmd = command;
                }
                void run(){};
};

class Link: public Words
{
       protected:
                Words* L;
                Words* R;

       public:
                Link(){};
                void run() = 0;
};

class Line: public Words
{
        protected:
                vector<Words*> log;
                vector<string> comments;
 
        public:
                Line(){};
                void memorize(string){};
};



#endif
