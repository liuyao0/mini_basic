#include "tokenizer.h"
#include <iostream>
void Tokenizer::trim(string &str)
{
    while((!str.empty())&&(std::isspace(str[0])))
        str.erase(0,1);
    while((!str.empty())&&(std::isspace(str[str.length()-1])))
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
        //std::cout<<str<<std::endl;
        trim(str);
        // if(str[0]==',')
        // {
        //     str.erase(0,1);
        //     continue;
        // }
        // if(str[0]=='"'&&str.length()>1)
        // {
        //     int i;
        //     for(i=1;i<str.length();i++)
        //         if(str[i]=='"')
        //         {
        //             tokens.push_back(str.substr(0,i+1));
        //             str.erase(0,i+1);
        //             break;
        //         }
        //     if(i!=str.length())
        //         continue;
        // }
        // if(str[0]=='\''&&str.length()>1)
        // {
        //     int i;
        //     for(i=1;i<str.length();i++)
        //         if(str[i]=='"')
        //         {
        //             tokens.push_back(str.substr(0,i+1));
        //             str.erase(0,i+1);
        //             break;
        //         }
        //     if(i!=str.length())
        //         continue;
        // }
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

            if(temp[i-1]=='>'||temp[i-1]=='='||temp[i-1]=='<')
            {
                tokens.push_back(temp.substr(0,i-1));
                str.erase(0,i-1);
                break;
            }
            if(temp[i-1]==',')
            {
                int quotation1=0;
                int quotation2=0;
                for(int j=0;j<i-1;j++)
                {
                    if(temp[j]=='\'')
                        quotation1++;
                    if(temp[j]=='"')
                        quotation2++;
                }
                if(quotation1%2==0&&quotation2%2==0)
                {
                    tokens.push_back(temp.substr(0,i-1));
                    str.erase(0,i);
                    break;
                }
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
        trim(result[i]);
        iter++;
    }
    return result;
}
