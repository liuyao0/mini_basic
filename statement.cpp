#include "statement.h"
#include <QDebug>
bool Statement::waitForInput=false;
//Statement-------------------------------------
Statement::Statement()
{

}
int Statement::getlineno()
{
    return lineno;
}
Statement::~Statement()
{

}
//RemStmt-------------------------------------
RemStmt::RemStmt(string s):str(s)
{
    type=REM;
}

void RemStmt::excute(Widget *widget,EvaluationContext &context)
{
    return;
}

void RemStmt::printToUi(Widget *widget)
{
    string content=std::to_string(lineno);
    content+=" REM\n\t";
    content+=str;
    widget->ui->textEdit_tree->append(QString::fromStdString(content));
}

RemStmt::~RemStmt()
{

}
//LetStmt-------------------------------------
LetStmt::LetStmt()
{
    type=LET;
}
void LetStmt::excute(Widget *widget,EvaluationContext &context)
{
    if(exp->type()==CONSTANT_STRING||exp->type()==IDENTIFIER_STRING)
        context.setStringValue(var,exp->eval_string(context));
    else
        context.setValue(var,exp->eval(context));
}

void LetStmt::printToUi(Widget *widget)
{
    string content=std::to_string(lineno);
    content+=" LET =\n\t";
    content+=var;
    content+="\n";
    if(exp->type()!=COMPOUND)
        content+='\t';
    content+=exp->toString();
    widget->ui->textEdit_tree->append(QString::fromStdString(content));
}

LetStmt::~LetStmt()
{
    delete exp;
}
//PrintStmt-------------------------------------
PrintStmt::PrintStmt()
{
    type=PRINT;
}
void PrintStmt::excute(Widget *widget,EvaluationContext &context)
{

    widget->ui->textEdit_output->append(QString::number(exp->eval(context)));
}

void PrintStmt::printToUi(Widget *widget)
{
    string content=std::to_string(lineno);
    content+=" PRINT";
    content+="\n";
    if(exp->type()!=COMPOUND)
        content+='\t';
    content+=exp->toString();
    widget->ui->textEdit_tree->append(QString::fromStdString(content));
}

PrintStmt::~PrintStmt()
{
    delete exp;
}
//Printf--------------------------------------------
PrintfStmt::PrintfStmt(){}
void PrintfStmt::excute(Widget *widget,EvaluationContext &context)
{
    int argc=argv.size(),i=0;
    string str=format;
    for(i=0;i<argc;i++)
    {
        string::size_type pos=str.find("{}");
        if(pos==str.npos)
            throw(InvalidExpression(format));
        str.erase(pos,2);
        if(context.isDefined_String(argv[i]))
        {
            str.insert(pos,context.getStringValue(argv[i]));
            continue;
        }
        else
        {
            Expression* exp=Expression::ExpfromString(argv[i]);
            str.insert(pos,std::to_string(exp->eval(context)));
            delete exp;
            continue;
        }
    }
    widget->ui->textEdit_output->append(QString::fromStdString(str));
}

void PrintfStmt::printToUi(Widget *widget)
{

    string content=std::to_string(lineno);
    content+=" PRINTF";
    content+="\n\t";
    content+="\"";
    content+=format;
    content+="\"";
    for(int i=0;i<argv.length();i++)
    {
        Expression *exp;
        content+="\n";
        exp=Expression::ExpfromString(argv[i]);
        if(exp->type()!=COMPOUND)
            content+="\t";
        content+=exp->toString();
        delete exp;
        content+="\n";
    }
    widget->ui->textEdit_tree->append(QString::fromStdString(content));
}
PrintfStmt::~PrintfStmt(){}
//InputStmt-------------------------------------
InputStmt::InputStmt(string s):var(s)
{
    type=INPUT_;
}
void InputStmt::excute(Widget *widget,EvaluationContext &context)
{
    widget->ui->lineEdit->setText("? ");
    bool toIntOK;
    waitForInput=true;
    widget->ui->pushButton_clear->setDisabled(true);
    widget->ui->pushButton_run->setDisabled(true);
    widget->ui->pushButton_debug->setDisabled(true);
    widget->ui->pushButton_load->setDisabled(true);
    while(waitForInput)
    {
        QCoreApplication::processEvents();
    }
    widget->ui->pushButton_clear->setDisabled(false);
    widget->ui->pushButton_run->setDisabled(false);
    widget->ui->pushButton_debug->setDisabled(false);
    widget->ui->pushButton_load->setDisabled(false);
    QString content=widget->ui->lineEdit->text();
    widget->ui->lineEdit->clear();
    if(!(content[0]=='?'&&content[1]==' '))
        throw(InvalidInput());
    content.remove(0,2);
    int val;
    val=content.toInt(&toIntOK,10);
    if(!toIntOK)
        throw(InvalidInput());
    context.setValue(var,val);
}

void InputStmt::printToUi(Widget *widget)
{
    string content=std::to_string(lineno);
    content+=" INPUT";
    content+="\n\t";
    content+=var;
    widget->ui->textEdit_tree->append(QString::fromStdString(content));
}

InputStmt::~InputStmt()
{

}
//InputsStmt-------------------------------------
InputsStmt::InputsStmt(string s):var(s)
{
    type=INPUTS;
};
void InputsStmt::excute(Widget *widget,EvaluationContext &context)
{
    widget->ui->lineEdit->setText("? ");
    waitForInput=true;
    widget->ui->pushButton_clear->setDisabled(true);
    widget->ui->pushButton_run->setDisabled(true);
    widget->ui->pushButton_debug->setDisabled(true);
    widget->ui->pushButton_load->setDisabled(true);

    while(waitForInput)
    {
        QCoreApplication::processEvents();
    }
    widget->ui->pushButton_clear->setDisabled(false);
    widget->ui->pushButton_run->setDisabled(false);
    widget->ui->pushButton_debug->setDisabled(false);
    widget->ui->pushButton_load->setDisabled(false);
    QString content=widget->ui->lineEdit->text();
    widget->ui->lineEdit->clear();
    if(!(content[0]=='?'&&content[1]==' '))
        throw(InvalidInput());
    for(int i=0;i<content.length();i++)
        if(content[i]=='\''||content[i]=='"')
            throw(InvalidInput());
    content.remove(0,2);
    context.setStringValue(var,content.toStdString());
}
void InputsStmt::printToUi(Widget *widget)
{
    string content=std::to_string(lineno);
    content+=" INPUTS";
    content+="\n\t";
    content+=var;
    widget->ui->textEdit_tree->append(QString::fromStdString(content));
}
InputsStmt::~InputsStmt(){}
//GotoStmt-------------------------------------
GotoStmt::GotoStmt()
{

}
void GotoStmt::excute(Widget *widget, EvaluationContext &context)
{
    program::PC=dir;
}
void GotoStmt::printToUi(Widget *widget)
{
    string content=std::to_string(lineno);
    content+=" GOTO";
    content+="\n\t";
    content+=std::to_string(dir);
    widget->ui->textEdit_tree->append(QString::fromStdString(content));
}
GotoStmt::~GotoStmt()
{

}
//IfStmt-------------------------------------
IfStmt::IfStmt()
{

}
void IfStmt::excute(Widget *widget,EvaluationContext &context)
{
    int val1=exp1->eval(context);
    int val2=exp2->eval(context);
    bool jmp;
    switch(op[0])
    {
       case '>':
        jmp=val1>val2;
        break;
       case '=':
        jmp=val1==val2;
        break;
       case '<':
        jmp=val1<val2;
        break;
    }

        program::PC=jmp?dir:0;        
        return;
}

void IfStmt::printToUi(Widget *widget)
{
    string content=std::to_string(lineno);
    content+=" IF THEN";
    content+="\n";
    if(exp1->type()!=COMPOUND)
        content+='\t';
    content+=exp1->toString();
    content+="\n\t";
    content+=op;
    content+='\n';
    if(exp2->type()!=COMPOUND)
        content+='\t';
    content+=exp2->toString();
    content+="\n\t";
    content+=std::to_string(dir);
    widget->ui->textEdit_tree->append(QString::fromStdString(content));
}

IfStmt::~IfStmt()
{
    if(exp1)
        delete exp1;
    if(exp2)
        delete exp2;
}
//EndStmt-------------------------------------
EndStmt::EndStmt()
{
    type=END;
}
void EndStmt::excute(Widget *widget,EvaluationContext &context)
{
}
void EndStmt::printToUi(Widget *widget)
{
    string content=std::to_string(lineno);
    content+=" END";
    widget->ui->textEdit_tree->append(QString::fromStdString(content));
}
EndStmt::~EndStmt()
{

}
