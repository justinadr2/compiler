#include <iostream>

#include "ast.h"

ASTConstantNode::ASTConstantNode(double val) : val(val) {}

ASTOpNode::ASTOpNode(char op, ASTNode* left, ASTNode* right) : op(op), left(left), right(right) {}

ASTOpNode::~ASTOpNode()
{
    delete left;
    delete right;
}

ASTVariableNode::ASTVariableNode(const string& name) : name(name), initialized(false) {}

ASTAssignmentNode::ASTAssignmentNode(ASTVariableNode* left, ASTNode* right) : left(left), right(right) {}

ASTAssignmentNode::~ASTAssignmentNode()
{
    delete left;
    delete right;
}

ASTPrintNode::ASTPrintNode(ASTNode* expr) : expr(expr) {}

ASTPrintNode::~ASTPrintNode()
{ 
    delete expr;
}

ASTFunctionNode::ASTFunctionNode(vector<ASTNode*>& statements) : statements(statements) {}

ASTFunctionNode::~ASTFunctionNode()
{
    for (ASTNode* statement : statements)
        delete statement;
}


double ASTConstantNode::evaluate()
{
    return val;
}

double ASTOpNode::evaluate()
{
    double left_val = left->evaluate();
    double right_val = right->evaluate();

    switch (op)
    {
        case '+': return left_val + right_val;
        case '-': return left_val - right_val;
        case '*': return left_val * right_val;
        case '/':
            if (right_val == 0)
            {
                cout << "Fatal error: division by zero\n";
                exit(1);
            }
            return left_val / right_val;
        default:
            cout << "Unknown operator: " << op << '\n';
            exit(1);
    }
}

double ASTVariableNode::evaluate()
{
    if (symbolTable.find(name) == symbolTable.end())
    {
        cout << "Fatal error: '" << name << "' is undefined\n";
        exit(1);
    }
    return symbolTable[name];
}

double ASTAssignmentNode::evaluate()
{
    double val = right->evaluate();
    symbolTable[left->name] = val;
    left->initialized = true;
    return val;
}

double ASTPrintNode::evaluate()
{
    double val = expr->evaluate();
    std::cout << "Out: " << val << "\n";
    return val;
}

double ASTFunctionNode::evaluate()
{
    for (ASTNode* statement : statements)
        statement->evaluate();

    return 0; // function exited successfully
}


ASTNode* CreateConstantNode(double val)
{
    return new ASTConstantNode(val);
}

ASTNode* CreateOpNode(char op, ASTNode* left, ASTNode* right)
{
    return new ASTOpNode(op, left, right);
}

ASTNode* CreateVariableNode(const string& name)
{
    return new ASTVariableNode(name);
}

ASTNode* CreateAssignmentNode(ASTVariableNode* left, ASTNode* right)
{
    return new ASTAssignmentNode(left, right);
}

ASTNode* CreatePrintNode(ASTNode* expr)
{
    return new ASTPrintNode(expr);
}

ASTNode* CreateFunctionNode(vector<ASTNode*>& statements)
{
    return new ASTFunctionNode(statements);
}