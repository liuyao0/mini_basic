#ifndef EXP_H
#define EXP_H

#include <string>
#include <cstring>
#include <map>
#include <stack>
#include <QString>
using std::stack;
using std::string;
using std::map;
enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND ,CONSTANT_STRING,IDENTIFIER_STRING};
class EvaluationContext;
//Exception type.
//Being thrown if a invalid expressions appears.
//The expression is be stored in exp.
class InvalidExpression{
public:
    string exp;
    InvalidExpression(string s):exp(s){}
};
//Exception type.
//Being thrown if a invalid identify appears.
//The identify is be stored in var.
class InvalidIdentify{
public:
    string var;
    InvalidIdentify(string s):var(s){}
};

//Exception type.
//Being thrown if a identify is used but not defined.
//The identify is be stored in inf.
class UnknownIdentify
{
public:
    string inf;
    UnknownIdentify(string name):inf(name){};
};

//Exception type.
//Being thrown if a divide by zero error occurs.
class Dividedbyzero
{};

class IllegalOperator{};

//A base class means expressions in programs.
class Expression
{
public:

   Expression();
   virtual ~Expression();
   //class:Expression
   //type:int
   //argument:
   //   EvaluationContext & context: the symbol table of the program
   //Evaluate the expression.
   virtual int eval(EvaluationContext & context);
   virtual string eval_string(EvaluationContext & context);
   //class:Expression
   //type:string
   //argument:
   //Convert the expression to a string
   virtual string toString() = 0;

   //class:Expression
   //type:ExpressionType
   //argument:
   //Return the type of the expression
   virtual ExpressionType type() = 0;

/* Getter methods for convenience */

   virtual int getConstantValue();
   virtual string getIdentifierName();
   virtual string getOperator();
   virtual Expression *getLHS();
   virtual Expression *getRHS();
   static Expression* ExpfromString(string str);
   static bool IllegalIdentify(string var);

};

/*
 * Class: ConstantExp
 * ------------------
 * This subclass represents a constant integer expression.
 */

class ConstantExp: public Expression
{

public:

   ConstantExp(int val);

   virtual int eval(EvaluationContext & context);

   virtual string toString();
   virtual ExpressionType type();

   virtual int getConstantValue();
   ~ConstantExp();
private:

   int value;

};

/*
 * Class: IdentifierExp
 * --------------------
 * This subclass represents a expression corresponding to a variable.
 */

class IdentifierExp: public Expression {

public:

   IdentifierExp(string str);

   virtual int eval(EvaluationContext & context);
   virtual string toString();
   virtual ExpressionType type();

   virtual string getIdentifierName();
   ~IdentifierExp();

private:
   string name;
};

/*
 * Class: CompoundExp
 * ------------------
 * This subclass represents a compound expression.
 */

class CompoundExp: public Expression
{
public:
   CompoundExp(string op_, Expression *lhs_, Expression *rhs_);
   virtual ~CompoundExp();

   virtual int eval(EvaluationContext & context);
   virtual string toString();
   virtual ExpressionType type();
   virtual string getOperator();
   virtual Expression *getLHS();
   virtual Expression *getRHS();
   void preOrder(string &str,int depth,Expression *exp);

private:
   string op;
   Expression *lhs, *rhs;
};
/*
 * Class: ConstantStringExp
 * ------------------
 * This subclass represents a constant string.
 */
class ConstantStringExp:public Expression
{
public:
    ConstantStringExp();
    ConstantStringExp(string str);
    virtual ExpressionType type();
    virtual ~ConstantStringExp();
    virtual string eval_string(EvaluationContext & context);
    virtual string toString();
private:
    string val;
};
/*
 * Class: IdentifierStringExp
 * ------------------
 * This subclass represents a constant string.
 */
class IdentifierStringExp:public Expression{
public:
    IdentifierStringExp();
    IdentifierStringExp(string str);
    virtual ~IdentifierStringExp();
    virtual ExpressionType type();
    virtual string eval_string(EvaluationContext & context);
    virtual string toString();
private:
    string var;
};

/*
 * Class: EvaluationContext
 * ------------------------
 * This class encapsulates the information that the evaluator needs to
 * know in order to evaluate an expression.
 *
 *
 */

class EvaluationContext
{
public:
   void setValue(string var, int value);
   void setStringValue(string var,string value);
   int getValue(string var);
   string getStringValue(string var);
   bool isDefined(string var);
   bool isDefined_String(string var);
   void clear();
   map<string,int> symbolTable;
   map<string,string> symbolTable_string;
};

#endif // EXP_H
