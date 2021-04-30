#ifndef Statement_H
#define Statement_H

#include <QTextEdit>
#include <QLineEdit>
#include <string>
#include <QString>
#include <ui_widget.h>
#include "exp.h"
#include "widget.h"

using std::string;
class InvalidInput{};
enum StatementType {REM,LET,PRINT,INPUT_,IF,GOTO,END};
class program;
class Widget;
class LinenoOutofRange {};
class Statement
{
public:
    Statement();
    static bool waitForInput;
    int lineno;
    StatementType type;
    virtual void printToUi(Widget *widget)=0;
    virtual void excute(Widget *widget,EvaluationContext &context)=0;
    virtual int getlineno();
    virtual ~Statement();
    static Statement* fromString(int lineno,string str);
};

class RemStmt:public Statement
{
public:
    RemStmt(string s);
    string str;
    virtual void excute(Widget *widget,EvaluationContext &context);
    virtual void printToUi(Widget *widget);
    virtual ~RemStmt();
};

class LetStmt:public Statement
{
public:
    string var;
    Expression *exp;
    LetStmt();
    virtual void excute(Widget *widget,EvaluationContext &context);
    virtual void printToUi(Widget *widget);
    virtual ~LetStmt();
};

class PrintStmt:public Statement
{
public:
    PrintStmt();
    Expression *exp;
    virtual void excute(Widget *widget,EvaluationContext &context);
    virtual void printToUi(Widget *widget);
    ~PrintStmt();
};

class InputStmt:public Statement
{
public:
    bool FinishEdit;
    InputStmt(string s);
    string var;
    virtual void excute(Widget *widget,EvaluationContext &context);
    virtual void printToUi(Widget *widget);
    ~InputStmt();
public slots:
    void changeEditState()
    {
        FinishEdit=true;
    }


};
class GotoStmt:public Statement
{
public:
    int dir;
public:
    GotoStmt();
    virtual void excute(Widget *widget,EvaluationContext &context);
    virtual void printToUi(Widget *widget);
    ~GotoStmt();
};

class IfStmt:public Statement
{
public:
    Expression* exp1;
    Expression* exp2;
    string op;
    int dir;
public:
    IfStmt();
    virtual void excute(Widget *widget,EvaluationContext &context);
    virtual void printToUi(Widget *widget);
    ~IfStmt();
};

class EndStmt:public Statement
{
public:
    EndStmt();
    virtual void excute(Widget *widget,EvaluationContext &context);
    virtual void printToUi(Widget *widget);
    ~EndStmt();
};



#endif // Statement_H
