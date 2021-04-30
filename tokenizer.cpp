#include "tokenizer.h"

void Tokenizer::trim(string &str)
{
    while((!str.empty())&&(std::isspace(str[0])))
        str.erase(0,1);
    while((!str.empty())&&(std::isspace(str[str.length()])))
        str.erase(str.length()-1,1);
}


Tokenizer::Tokenizer()
{
}

void Tokenizer::addOrder(string str)
{
    auto iter=allOrder.find(str);
    if(iter!=allOrder.end())
        return;
    allOrder.insert(str);
}


void Tokenizer::deleteOrder(string str)
{
    auto iter=allOrder.find(str);
    if(iter!=allOrder.end())
        allOrder.erase(str);
}



string* Tokenizer::tokenize(string str,int &num)
{
    list<string> tokens;
    string* result;
    trim(str);
    while(str.length()>0)
    {

        trim(str);
        if(str.substr(0,1)==">"||str.substr(0,1)=="="||str.substr(0,1)=="<")
        {
            tokens.push_back(str.substr(0,1));
            str.erase(0,1);
            continue;
        }

        int i=1;
        while(true)
        {
            string temp=str.substr(0,i);
            if((allOrder.find(temp)!=allOrder.end())&&(str.length()>i)&&(std::isspace(str[i])))
            {
                tokens.push_back(temp);
                str.erase(0,i);
                break;
            }

            if((i>=6&&std::isspace(temp[i-6])&&std::isspace(temp[i-1])&&temp.substr(i-5,4)=="THEN"))
            {
                temp.erase(i-6);
                trim(temp);
                tokens.push_back(temp);
                tokens.push_back("THEN");
                str.erase(0,i);
                break;
            }

            if(i==str.length())
            {
                tokens.push_back(str);
                str.clear();
                break;
            }

            if((temp.substr(i-1,1)==">"||temp.substr(i-1,1)=="="||temp.substr(i-1,1)=="<"))
            {
                tokens.push_back(temp.substr(0,i-1));
                str.erase(0,i-1);
                break;
            }
            i++;
        }
    }
    num=tokens.size();
    if(num==0) return nullptr;
    result=new string[num];
    auto iter=tokens.begin();
    for(int i=0;i<num;i++)
    {
        result[i]=*iter;
        iter++;
    }
    return result;
}
