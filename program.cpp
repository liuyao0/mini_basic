#include "program.h"
#include <QDebug>
int program::PC=0;

program::program()
{
    tokenizer.addOrder(string("PRINT"));
    tokenizer.addOrder(string("INPUT"));
    tokenizer.addOrder(string("LET"));
    tokenizer.addOrder(string("IF"));
    tokenizer.addOrder(string("GOTO"));
    tokenizer.addOrder(string("END"));
    PC=0;
}

program::~program()
{
    clear();
}

//class:program
//type:void
//argument:
//Remove all statements in the program.
//Clear the symbol table.
void program::clear()
{
    context.clear();
    if(stats.empty())
        return;
    for(auto iter=stats.begin();iter!=stats.end();iter++)
        delete (*iter);
    stats.clear();
}

//class:program
//type:clear
//argument:
//  Widget *w
//  stc *sentence : the sentence needs parsing
//parse a sentence
Statement* program::parse_line(Widget *w,stc *sentence)
{
    int ln=sentence->lineno;
    if(ln>1000000||ln<0)
        throw(LinenoOutofRange());
    int n;
    string stat=sentence->str;
    string token[6];

    //REM---------------------------------------------------------------
    Tokenizer::trim(stat);
    qDebug()<<QString::number(ln)<<QString::fromStdString(stat);
    if(((stat.length()>3)&&(stat.substr(0,3)=="REM")&&(std::isspace(stat[3])))||(stat=="REM"))
    {
        RemStmt *rem;
        if(stat=="REM")
        {
            rem=new RemStmt("");
            rem->lineno=ln;
            rem->printToUi(w);
            stats.push_back(rem);
        }
        else
        {
            string s;
            s=stat.substr(3);
            Tokenizer::trim(s);
            rem=new RemStmt(s);
            rem->lineno=ln;
            rem->printToUi(w);

        };
        return rem;
    }
    string* temp_token=tokenizer.tokenize(sentence->str,n);
    if(n>6)
    {
        delete temp_token;
        throw(WrongStatement());
    }
    for(int i=0;i<n;i++)
        token[i]=temp_token[i];
    delete []temp_token;
    //LET----------------------------------------------------------------
    if(n==4&&token[0]=="LET"&&token[2]=="=")
    {
        string var=token[1];
        Expression* exp=Expression::ExpfromString(token[3]);
        LetStmt* let=new LetStmt;
        let->var=var;
        let->exp=exp;
        let->lineno=ln;
        let->printToUi(w);
        return let;

    }
    //INPUT--------------------------------------------------------------
    if(n==2&&token[0]=="INPUT")
    {
        string var=token[1];
        if(Expression::IllegalIdentify(var))
             throw(InvalidIdentify(var));
        InputStmt *input=new InputStmt(token[1]);
        input->lineno=ln;
        input->printToUi(w);
        return input;
    }
    //PRINT--------------------------------------------------------------
    if(n==2&&token[0]=="PRINT")
    {
        Expression* exp=Expression::ExpfromString(token[1]);
        PrintStmt *prt=new PrintStmt;
        prt->exp=exp;
        prt->lineno=ln;
        prt->printToUi(w);
        return prt;
    }
    //GOTO------------------------------------------------------------
    if(n==2&&token[0]=="GOTO")
    {
        bool toIntOK;
        int jumpto;
        jumpto=(QString::fromStdString(token[1])).toInt(&toIntOK,10);
        if(!toIntOK)
            throw(InvalidExpression(token[1]));
        GotoStmt *gotostmt=new GotoStmt;
        gotostmt->lineno=ln;
        gotostmt->dir=jumpto;
        gotostmt->printToUi(w);
        return gotostmt;
    }
    //IF----------------------------------------------------------------
    if(n==6&&token[0]=="IF"&&token[4]=="THEN")
    {
        bool toIntOK;
        int jumpto;

        Expression* exp1=Expression::ExpfromString(token[1]);
        Expression* exp2=Expression::ExpfromString(token[3]);
        string op=token[2];
        jumpto=(QString::fromStdString(token[5])).toInt(&toIntOK,10);
        if(!toIntOK)
            throw(InvalidExpression(token[5]));
        IfStmt *ifstmt=new IfStmt;
        ifstmt->lineno=ln;
        ifstmt->exp1=exp1;
        ifstmt->op=op;
        ifstmt->exp2=exp2;
        ifstmt->dir=jumpto;
        ifstmt->printToUi(w);
        return ifstmt;
    }
    //END--------------------------------------------------------------
    if(n==1&&token[0]=="END")
    {
        EndStmt *end_=new EndStmt;
        end_->lineno=ln;
        end_->printToUi(w);
        return end_;
    }
    throw(WrongStatement());
}

//class:program
//type:void
//Load program from buffer.
void program::load(Buffer *buffer,Widget *w)
{
    clear();
    auto iter=buffer->L.begin();    
    while(iter!=buffer->L.end())
    {
        Statement *stat;
        try
        {
            stat=parse_line(w,&(*iter));
            stats.push_back(stat);

        }
        catch (WrongStatement)
        {
            QString str=QString::number(iter->lineno);
            str+=" Wrong Statement '";
            str+=QString::fromStdString(iter->str);
            str+="'.";
            w->ui->textEdit_tree->append(str);

            EndStmt *end_;
            end_=new EndStmt;
            end_->lineno=iter->lineno;
            stats.push_back(end_);

        }
        catch (InvalidExpression(InvalidExp))
        {
            QString str=QString::number(iter->lineno);
            str+=" Invalid Expression '";
            str+=QString::fromStdString(InvalidExp.exp);
            str+="'.";
            w->ui->textEdit_tree->append(str);

            EndStmt *end_;
            end_=new EndStmt;
            end_->lineno=iter->lineno;
            stats.push_back(end_);
        }
        catch (LinenoOutofRange)
        {
            QString str="Lineno '";
            str+=QString::number(iter->lineno);
            str+="' out of range.";

            EndStmt *end_;
            end_=new EndStmt;
            end_->lineno=iter->lineno;
            stats.push_back(end_);
        }
        catch (InvalidIdentify(identify))
        {
            QString str=QString::number(iter->lineno);
            str+=" Invalid identify '";
            str+=QString::fromStdString(identify.var);
            str+="'.";
            w->ui->textEdit_tree->append(str);
            EndStmt *end_;
            end_=new EndStmt;
            end_->lineno=iter->lineno;
            stats.push_back(end_);
        }
        catch(std::out_of_range)
        {
            QString str=QString::number(iter->lineno);
            str+=" Wrong Statement '";
            str+=QString::fromStdString(iter->str);
            str+="'.";
            w->ui->textEdit_tree->append(str);

            EndStmt *end_;
            end_=new EndStmt;
            end_->lineno=iter->lineno;
            stats.push_back(end_);
        }
        iter++;
    }
}
//Run the program
void program::run(Widget *w)
{

    if(stats.empty())
        return;
    auto iter=stats.begin();
    while(true)
    {
        if(iter==stats.end())
            break;
        PC=0;
        try
        {
            (*iter)->excute(w,context);
        }
        catch (Dividedbyzero)
        {
            QString str="Line ";
            str+=QString::number((*iter)->lineno);
            str+=", divided by zero!";
            w->ui->textEdit_output->append(str);
            return;
        }
        catch (UnknownIdentify(unknownIdentify))
        {
            QString str="Line ";
            str+=QString::number((*iter)->lineno);
            str+=", unknown Identify '";
            str+=QString::fromStdString(unknownIdentify.inf);
            str+="'.";
            w->ui->textEdit_output->append(str);
            return;
        }


        if((*iter)->type==END)
            return;
        if(PC!=0)
        {
            auto jumpto=stats.begin();
            for(;jumpto!=stats.end();jumpto++)
                if((*jumpto)->getlineno()==PC)
                    break;
            if(jumpto==stats.end())
            {
                QString str="Lineno '";
                str+=QString::number(PC);
                str+="' doesn't exist.";
                w->ui->textEdit_output->append(str);
            }


            iter=jumpto;
        }
        else
            iter++;
    }
}

bool program::doImmCmd(string cmd,Widget *w)
{
    string *pretoken,token[4];
    int n;
    Tokenizer::trim(cmd);
    pretoken=tokenizer.tokenize(cmd,n);
    if(n>4)
        return false;
    for(int i=0;i<n;i++)
        token[i]=pretoken[i];
    delete []pretoken;
    if(n==4&&token[0]=="LET"&&token[2]=="=")
    {
        Expression *exp;
             exp=Expression::ExpfromString(token[3]);
        LetStmt let;
        let.var=token[1];
        let.exp=exp;
        let.lineno=0;
        let.excute(w,context);
        return true;
    }
    //INPUT--------------------------------------------------------------
    if(n==2&&token[0]=="INPUT")
    {
        if(Expression::IllegalIdentify(token[1]))
            throw(InvalidIdentify(token[1]));
        InputStmt input(token[1]);
        input.lineno=0;
        input.excute(w,context);
        return true;
    }
    //PRINT--------------------------------------------------------------
    if(n==2&&token[0]=="PRINT")
    {
        Expression *exp=Expression::ExpfromString(token[1]);
        PrintStmt prt;
        prt.exp=exp;
        prt.lineno=0;
        prt.excute(w,context);
        return true;
    }
    return false;
}
