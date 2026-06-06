#include <iostream>

#include "ast.h"


bool TrackSymbol::exists(string name)
{
    return find(variables.begin(), variables.end(), name) != variables.end();
}

void TrackSymbol::push(string name)
{
    variables.push_back(name);
}


Constant::Constant(string val) : val(val) {}

BinaryOp::BinaryOp(char op, Node* left, Node* right) : op(op), left(left), right(right) {}

BinaryOp::~BinaryOp()
{
    delete left;
    delete right;
}

DeclareVariable::DeclareVariable(string name, Node* expr) : name(name), expr(expr) {} 

DeclareVariable::~DeclareVariable()
{
    delete expr;
}

GetVariable::GetVariable(string name) : name(name) {}

Assignment::Assignment(GetVariable* left, Node* right) : left(left), right(right) {}

Assignment::~Assignment()
{
    delete left;
    delete right;
}

Function::Function(string name, vector<Node*>& block) : name(name), block(block) {}

Function::~Function()
{
    for (auto& statement : block)
        delete statement;
}


string Constant::build(Builder& builder, TrackSymbol& tracker)
{
    return val;
}

string BinaryOp::build(Builder& builder, TrackSymbol& tracker)
{
    string left_val = left->build(builder, tracker);
    string right_val = right->build(builder, tracker);

    string reg = builder.get_new_temp();
    string instruction = reg + " = " + left_val + ' ' + op + ' ' + right_val;
    builder.emit(instruction);
    return reg;
}

string DeclareVariable::build(Builder& builder, TrackSymbol& tracker)
{
    if (tracker.exists(name))
    {
        cout << "Fatal error: variable '" << name << "' already exists\n";
        exit(1);
    }

    tracker.push(name);
    if (expr)
    {
        string val = expr->build(builder, tracker);
        string instruction = name + " = " + val;
        builder.emit(instruction);
    }
    return name;
}

string GetVariable::build(Builder& builder, TrackSymbol& tracker)
{
    if (!tracker.exists(name))
    {
        cout << "Fatal error: undeclared variable '" << name << "'\n";
        exit(1);
    }
    return name;
}

string Assignment::build(Builder& builder, TrackSymbol& tracker)
{
    if (tracker.exists(left->name))
    {
        string var = left->build(builder, tracker);
        string expr = right->build(builder, tracker);
        string instruction = var + " = " + expr;
        builder.emit(instruction);
        return left->name;
    }
    cout << "Fatal error: undeclared variable '" << left->name << "'\n";
    exit(1);
}

string Function::build(Builder& builder, TrackSymbol& tracker)
{
    for (auto& statement : block)
        statement->build(builder, tracker);

    builder.emit("ret 0");
    return "";
}