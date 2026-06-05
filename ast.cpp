#include <iostream>

#include "ast.h"

// for tracking if the symbol exists
class TrackSymbol
{
public:
    vector<string> variables;

    bool exists(string name)
    {
        return find(variables.begin(), variables.end(), name) != variables.end();
    }

    void push(const string& name)
    {
        variables.push_back(name);
    }

} trackSymbol;


Constant::Constant(double val) : val(val) {}

BinaryOp::BinaryOp(char op, Node* left, Node* right) : op(op), left(left), right(right) {}

BinaryOp::~BinaryOp()
{
    delete left;
    delete right;
}

GetVariable::GetVariable(string name) : name(name) {}

Assignment::Assignment(GetVariable* left, Node* right) : left(left), right(right) {}

Assignment::~Assignment()
{
    delete left;
    delete right;
}

DeclareVariable::DeclareVariable(string name, Node* expr) : name(name), expr(expr) {} 

DeclareVariable::~DeclareVariable()
{
    delete expr;
}



string Constant::build(Builder& builder)
{
    return to_string(val);
}

string BinaryOp::build(Builder& builder)
{
    string left_val = left->build(builder);
    string right_val = right->build(builder);

    string reg = builder.get_new_temp();
    string instruction = reg + " = " + left_val + ' ' + op + ' ' + right_val;
    builder.emit(instruction);
    return reg;
}

string DeclareVariable::build(Builder& builder)
{
    if (trackSymbol.exists(name))
    {
        cout << "Fatal error: variable '" << name << "' already exists\n";
        exit(1);
    }
    trackSymbol.push(name);
    if (expr)
    {
        string val = expr->build(builder);
        string instruction = name + " = " + val;
        builder.emit(instruction);
    }
    return name;
}

string GetVariable::build(Builder& builder)
{
    if (!trackSymbol.exists(name))
    {
        cout << "Fatal error: undeclared variable '" << name << "'\n";
        exit(1);
    }
    return name;
}

string Assignment::build(Builder& builder)
{
    if (trackSymbol.exists(left->name))
    {
        string val = right->build(builder);
        string instruction = left->name + " = " + val;
        builder.emit(instruction);
        return left->name;
    }
    cout << "Fatal error: undeclared variable '" << left->name << "'\n";
    exit(1);
}



Node* CreateConstantNode(double val)
{
    return new Constant(val);
}

Node* CreateBinaryOpNode(char op, Node* left, Node* right)
{
    return new BinaryOp(op, left, right);
}

Node* CreateDeclareVariableNode(string name)
{
    return new DeclareVariable(name);
}

Node* CreateDeclareVariableNode(string name, Node* expr)
{
    return new DeclareVariable(name, expr);
}

Node* CreateGetVariableNode(string name)
{
    return new GetVariable(name);
}

Node* CreateAssignmentNode(GetVariable* left, Node* right)
{
    return new Assignment(left, right);
}
