#ifndef PROGRAM_H
#define PROGRAM_H
#include <list>
#include "statement.h"
#include "tokenizer.h"
#include "Buffer.h"
#include "ui_widget.h"
#include "widget.h"
using std::list;
class stc;
class Widget;
class WrongStatement{};
class NoExistLineno
{
private:
    int lineno;
public:
    NoExistLineno(int l):lineno(l){}
    int getLlineno(){return lineno;}
};

class Statement;
//the class implement the basic interpreter.
//stats stores all statements.
//tokenizer is a tokenizer.
//context is a symbol table.
//PC stores the next statement need to be excuted.
class program
{
private:
    list<Statement*> stats;
    list<Statement*>::iterator currentStat;
    Tokenizer tokenizer;
    EvaluationContext context;
    list<int> wrongLine;
public:
    bool onDebug;
    static int PC;
    program();
    ~program();
    void clear();
    void load(Buffer *buffer,Widget *w);
    Statement* parse_line(Widget *w,stc *sentence);
    void setHighlight(Widget* w,Statement *stat=NULL);
    void run(Widget *w);
    bool doImmCmd(string cmd,Widget *w);
    void debug(Widget *w);
    void printVar(Widget *w);
};

#endif // PROGRAM_H
