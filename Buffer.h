#pragma once

#include <string>
#include <list>
#include <sstream>
#include <QTextStream>
#include <QTextEdit>
#include <QString>

using std::string;
using std::list;
using std::stringstream;
//This class means sentence.
//lineno saves the order number of setence,
//and str saves content of sentences.
class stc //sentence
{
public:
    long lineno;
    string str;
    stc();
    stc(long l,string s);
};

//This class saves sentences by a link list.
class Buffer
{
public:
    list<stc> L;
    long currentLength();
    stc at(int i);
    void insert(stc &s);
    void remove(long l);
    void printToUi(QTextEdit *qte);
    void dispatchCmd(string cmd);
    void clear();

};
