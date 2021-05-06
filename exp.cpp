#include "exp.h"
#include <queue>
#include <QDebug>
using std::queue;
//Expression--------------------------------------------
Expression::Expression()
{

}

Expression::~Expression()
{

}
int Expression::getConstantValue()
{
    return -1;
}
string Expression::getIdentifierName()
{
    return "";
}
string Expression::getOperator()
{
    return "";
}
Expression *Expression::getLHS()
{
    return nullptr;
}
Expression *Expression::getRHS()
{
    return nullptr;
}

//class:Expression
//type:bool
//argument:
//  string var : identify needs judging
//Judge whether the identify is illegal.If so, return true. If no, return fasle.
bool Expression::IllegalIdentify(string var)
{
    if(var[0]=='-')    //ignore '-'
        var.erase(0,1);
    if(!((var[0]>='A'&&var[0]<='Z')||       //the first character need to be a letter or '_'
         (var[0]>='a'&&var[0]<='z')||
         (var[0]=='_')))
        return true;
    for(int i=1;i<var.length();i++)
        if(!((var[i]>='A'&&var[i]<='Z')||   //only letters, digits and '_' can appear in an identify
             (var[i]>='a'&&var[i]<='z')||
             (var[i]=='_')||
             (var[i]>='0'&&var[i]<='9')))
            return true;
    bool flag=false;
    for(int i=0;i<var.length();i++)
        if((var[i]>='A'&&var[i]<='Z')||  //at least have one letter
            (var[i]>='a'&&var[i]<='z'))
         flag=true;
        if(!flag)
            return true;
    return false;
}

//ConstantExp--------------------------------------------
ConstantExp::ConstantExp(int val):value(val){};

int ConstantExp::eval(EvaluationContext &context)
{
    return value;
}

string ConstantExp::toString()
{
    return std::to_string(value);
}

ExpressionType ConstantExp::type()
{
    return CONSTANT;
}

int ConstantExp::getConstantValue()
{
    return value;
}

ConstantExp::~ConstantExp()
{
}
//IdentifierExp--------------------------------------------
IdentifierExp::IdentifierExp(string str):name(str){};

int IdentifierExp::eval(EvaluationContext &context)
{
    return context.getValue(name);
}

string IdentifierExp::toString()
{
    return name;
}

ExpressionType IdentifierExp::type()
{
    return IDENTIFIER;
}

string IdentifierExp::getIdentifierName()
{
    return name;
}
IdentifierExp::~IdentifierExp()
{

}

//CompoundExp--------------------------------------------
CompoundExp::CompoundExp(string op_, Expression *lhs_, Expression *rhs_):op(op_),lhs(lhs_),rhs(rhs_){};

CompoundExp::~CompoundExp()
{
    if(lhs->type()==COMPOUND)
        delete(lhs);
    if(rhs->type()==COMPOUND)
        delete(rhs);
}

int CompoundExp::eval(EvaluationContext & context)
{
   int right = rhs->eval(context);
   if (op == "=")
   {
      context.setValue(lhs->getIdentifierName(), right);
      return right;
   }
   int left = lhs->eval(context);
   if (op == "+") return left + right;
   if (op == "-") return left - right;
   if (op == "*") return left * right;
   if (op == "**")
   {
      int ans=1;
      for(int i=0;i<right;i++)
        ans*=left;
      return ans;
   }
   if (op == "/") {
      if (right == 0) throw(Dividedbyzero());
      return left / right;
   }
   throw(IllegalOperator());
   return 0;
}

ExpressionType CompoundExp::type()
{
    return COMPOUND;
}

string CompoundExp::getOperator()
{
    return op;
}

Expression* CompoundExp::getLHS()
{
    return lhs;
}

Expression* CompoundExp::getRHS()
{
    return rhs;
}
string CompoundExp::toString()
{
    int depth=1;
    string str="";
    queue<Expression*> Q; //used to traverse the expression tree.
    Expression *last;
    Expression *nlast;
    last=nlast=this;
    Q.push(this);
    while(!Q.empty())
    {
        Expression* exp=Q.front();
        Q.pop();
        for(int i=0;i<depth;i++) //indentation
            str+='\t';
        if(exp->type()==COMPOUND)
        {
            str+=exp->getOperator();
            str+='\n';
            Q.push(exp->getLHS());
            nlast=exp->getLHS();
            Q.push(exp->getRHS());
            nlast=exp->getRHS();
        }
        else
        {
            str+=exp->toString();
            str+='\n';
        }

        if(last==exp) //if need to open a new line.
        {
            last=nlast;
            depth++;
        }
    }
    return str;
}


//EvaluationContext--------------------------------------------

//class:setValue
//type:void
//argument:
//  string var : the identify of the variable need setting value
//  int value : the set value
//Set a value of a variable.If the identify doesn't exist, it will
//be created.
void EvaluationContext::setValue(string var, int value)
{
    auto iter=symbolTable_string.find(var);
    if(iter!=symbolTable_string.end())
        symbolTable_string.erase(iter);
    symbolTable[var]=value;
}

void EvaluationContext::setStringValue(string var, string value)
{
    auto iter=symbolTable.find(var);
    if(iter!=symbolTable.end())
        symbolTable.erase(iter);
    symbolTable_string[var]=value;
}
//class:EvaluationContext
//type:int
//argument:
//  string var : the identify of the variable whose value is needed.
//Return the value of a variable.If the identify doesn't exist, a
//exception will be thrown.
int EvaluationContext::getValue(string var)
{
    bool neg=false; //whether is negetive
    if(var[0]=='-')
    {
        var=var.substr(1);
        neg=true;
    }
    auto iter=symbolTable.find(var);
    if(iter==symbolTable.end())
        throw(UnknownIdentify(var));
    return neg?-iter->second:iter->second;
}

string EvaluationContext::getStringValue(string var)
{
    auto iter=symbolTable_string.find(var);
    if(iter==symbolTable_string.end())
        throw(UnknownIdentify(var));
    return iter->second;
}


bool EvaluationContext::isDefined(string var)
{
    auto iter=symbolTable.find(var);
    if(iter==symbolTable.end())
        return false;
    return true;
}

bool EvaluationContext::isDefined_String(string var)
{
    auto iter=symbolTable_string.find(var);
    if(iter==symbolTable_string.end())
        return false;
    return true;
}

void EvaluationContext::clear()
{
    symbolTable.clear();
}

//--------------------------------------------------------------

//return the precedence of an operator.If the operator doesn't exist, return -1.
int precedence(string str)
{
    if(str=="+"||str=="-") return 2;
    if(str=="*"||str=="/") return 3;
    if(str=="**") return 4;
    return -1;
}

bool isIdentify(char ch)
{
    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_'||(ch>='0'&&ch<='9'))
        return true;
    else
        return false;
}

bool isDigit(char ch)
{
    if((ch>='0'&&ch<='9'))
        return true;
    else
        return false;
}

//remove the space at in beginning of a string.
void trim(string &str)
{
    while((!str.empty())&&(str[0]==' '||str[0]=='\t'))
        str.erase(0,1);
}

//class:Expression
//type:Expression*
//argument:
//  string str : the expression
//construct a expression from a string.
Expression* Expression::ExpfromString(string str)
{
    int i=0;
    while(i<str.length())
    {
        if(i==0&&str[i]=='-')
        {
            str.insert(0,"0");
            i+=2;
            continue;
        }

        if(i!=0&&str[i-1]=='('&&str[i]=='-')
        {
            str.insert(i,"0");
            i+=2;
            continue;
        }
        i++;
    }
    qDebug()<<QString::fromStdString(str);
    bool flag=true;     //after a '(' or the beginning of expression.
    bool negflag=false;
    string preStr=str;
    stack<Expression*> operands;
    stack<string> operators;
    trim(str);
    auto striter=str.begin();
    while(str.length()>0)
    {
       // qDebug()<<QString::fromStdString(str);
        size_t numlen;
        trim(str);
        if(*striter=='-'&&flag)
        {
            str.erase(0,1);
            trim(str);
            negflag=true;
        }
        if(isDigit(*striter))  //If is a number
        {
            trim(str);
            int value=std::stoi(str,&numlen);
            if(negflag)
            {
                value=-value;
                negflag=false;
            }
            ConstantExp* constant=new ConstantExp(value);
            operands.push(constant);
            while(str.length()>0&&isDigit(str[0]))
                str.erase(0,1);
            trim(str);
            if(!str.empty())
                if(str[0]!='+'&&str[0]!='-'&&str[0]!='*'&&str[0]!='/'&&str[0]!='('&&str[0]!=')')  //数字
                    throw(InvalidExpression(preStr));
            flag=false;
            continue;
        }
        if(isIdentify(*striter)) //if is a variable
        {
            int i=0;
            if(negflag)
            {
                str="-"+str;
                negflag=false;
                i++;
            }
            while(isIdentify(str[i])&&i<str.length())
                i++;
            string varIdent=str.substr(0,i);
            if(IllegalIdentify(varIdent))
                throw(InvalidIdentify(varIdent));
            str.erase(0,i);
            IdentifierExp *exp=new IdentifierExp(varIdent);
            operands.push(exp);
            flag=false;
            continue;
        }
        else
        {
            string oper;                                 //neither a variable nor a number---operator
            if(str.length()>=2&&str.substr(0,2)=="**")
            {
                oper="**";
                str.erase(0,2);
            }
            else
            {
                oper=str.substr(0,1);
                if(str.length()>=2&&(str[1]=='-')&&((str[0]!='(')&&(str[0]!=')')))
                    throw(InvalidExpression(preStr));
                str.erase(0,1);

            }

            if(oper=="(")
            {
                operators.push(oper);
                flag=true;
                continue;
            }

            if(oper==")")
            {
                while(true)
                {
                    if(operators.empty())
                        throw(InvalidExpression(preStr));
                    string top_oper=operators.top();
                    operators.pop();
                    if(top_oper=="(")
                        break;
                    Expression *operand_a,*operand_b;
                    CompoundExp *operand_result;
                    if(operands.empty())
                        throw(InvalidExpression(preStr));
                    operand_b=operands.top();
                    operands.pop();
                    if(operands.empty())
                        throw(InvalidExpression(preStr));
                    operand_a=operands.top();
                    operands.pop();
                    operand_result=new CompoundExp(top_oper,operand_a,operand_b);
                    operands.push(operand_result);
                }
                flag=false;
                continue;
            }

            while((!operators.empty())&&precedence(oper)<=precedence(operators.top()))
            {
                string top_oper=operators.top();
                operators.pop();
                if(top_oper=="(")
                    break;
                Expression *operand_a,*operand_b;
                CompoundExp *operand_result;
                if(operands.empty())
                    throw(InvalidExpression(preStr));
                operand_b=operands.top();
                operands.pop();
                if(operands.empty())
                    throw(InvalidExpression(preStr));
                operand_a=operands.top();
                operands.pop();
                operand_result=new CompoundExp(top_oper,operand_a,operand_b);
                operands.push(operand_result);
				continue;
            }

			operators.push(oper);
            flag=false;
			continue;
        }
    }

    while(!operators.empty())    //pushing comes to an end
    {
        string top_oper=operators.top();
        operators.pop();
        if(top_oper=="(")
            break;
        Expression *operand_a,*operand_b;
        CompoundExp *operand_result;
        if(operands.empty())
            throw(InvalidExpression(preStr));
        operand_b=operands.top();
        operands.pop();
        if(operands.empty())
            throw(InvalidExpression(preStr));
        operand_a=operands.top();
        operands.pop();
        operand_result=new CompoundExp(top_oper,operand_a,operand_b);
        operands.push(operand_result);
    }

    return operands.top();
}

