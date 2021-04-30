#ifndef TOKENIZER_H
#define TOKENIZER_H
#include<string>
#include<set>
#include<list>
using std::string;
using std::set;
using std::list;
enum tokenState{expression,instr};
class Tokenizer
{
    set<string> allOrder;
public:
    Tokenizer();
    void addOrder(string str);
    void deleteOrder(string str);
    string* tokenize(string str,int &num);
    void print();
    static void trim(string &str);
};

#endif // TOKENIZER_H
