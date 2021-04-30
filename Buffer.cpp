#include "Buffer.h"

stc::stc()
{
    lineno=0;
}

stc::stc(long l,string s):lineno(l),str(s){}

//class:Buffer
//type:long
//argument:
//Return current length of sentences' list.
long Buffer::currentLength()
{
    return L.size();
}

//class:Buffer
//type:void
//argument:
//  stc &s :  sentence needs inserting.
//Insert a sentence to link list L.If the order number has existed, the content will be replaced.
void Buffer::insert(stc &s)
{
    if(currentLength()==0)
    {
        L.push_front(s);
        return;
    }
    auto iter=L.begin();

    while(iter->lineno<s.lineno&&iter!=L.end())
        iter++;

    if(iter==L.end())
    {
        L.push_back(s);
        return;
    }

    if(s.lineno==iter->lineno)
    {
        iter->str=s.str;
        return;
    }

    L.insert(iter,s);
}

//class:Buffer
//type:void
//argument:
//  long l : the order number of the sentence needing removing.
//Remove the sentence whose order number is lineno.
void Buffer::remove(long l)
{
    auto iter=L.begin();
    while(iter->lineno!=l&&iter!=L.end())
        iter++;
    if(iter==L.end())
        return;
    L.erase(iter);

}

//class:Buffer
//type:void
//argument:
//  QTextEdit *qte : where the content of sentences will be display
//Print the content of sentences to UI.
void Buffer::printToUi(QTextEdit *qte)
{
    qte->clear();
    auto iter=L.begin();
    while(iter!=L.end())
    {
        QString temp=QString::number(iter->lineno);
        temp+=' ';
        temp+=QString::fromStdString(iter->str);
        qte->append(temp);
        iter++;
    }
}

//class:Buffer
//type:void
//argument:
//  string cmd : command needs dispatching
//Dispatch the command about buffer.
void Buffer::dispatchCmd(string cmd)
{
    if(cmd[0]<'0'||cmd[0]>'9') //if isn't insert/remove/change
        return;
    stringstream ss(cmd);
    long lineno;
    ss>>lineno;   //get the order number
    if(ss.eof())  //if is remove
    {
        remove(lineno);
        return;
    }
    auto iter=cmd.begin();
    while(*iter>='0'&&*iter<='9')
        iter++;
    while(std::isspace(*iter))
        iter++;
    iter--;
    cmd.erase(cmd.begin(),iter);
    stc s(lineno,cmd);
    insert(s);
}

//class:Buffer
//type:void
//argument:
//Clear all sentences.
void Buffer::clear()
{
    L.clear();
}
