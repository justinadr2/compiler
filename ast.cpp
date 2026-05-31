#include <iostream>

#include "ast.h"

// for tracking if the symbol is initialized
class TrackSymbol
{
public:
    vector<pair<string, bool>> vars;

    TrackSymbol() 
    {
        vars.push_back({"", false});
    }

    bool exists(const string& name)
    {
        return scan(name) != nullptr;
    }

    bool initialized(const string& name)
    {
        auto var_ptr = scan(name);
        if (var_ptr)
            return var_ptr->second;
        return false;
    }

    void set_initialized(const string& name)
    {
        auto var_ptr = scan(name);
        if (var_ptr)
            var_ptr->second = true;
    }

    pair<string, bool>* scan(const string& name)
    {
        for (int i = 0; i < vars.size(); i++)
        {
            if (vars[i].first == name)
                return &vars[i];
        }
        return nullptr;
    }
} trackSymbol;


ASTConstantNode::ASTConstantNode(double val) : val(val) {}

ASTOpNode::ASTOpNode(char op, ASTNode* left, ASTNode* right) : op(op), left(left), right(right) {}

ASTOpNode::~ASTOpNode()
{
    delete left;
    delete right;
}

ASTVariableNode::ASTVariableNode(const string& name) : name(name) 
{
    if (!trackSymbol.exists(name))
        trackSymbol.vars.push_back({name, false});

}

ASTAssignmentNode::ASTAssignmentNode(ASTVariableNode* left, ASTNode* right) : left(left), right(right) {}

ASTAssignmentNode::~ASTAssignmentNode()
{
    delete left;
    delete right;
}

ASTDeclarationNode::ASTDeclarationNode(const string& name) : name(name) 
{
    if (!trackSymbol.exists(name))
        trackSymbol.vars.push_back({name, false});
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
    if (trackSymbol.initialized(name))
        return symbolTable[name];
    
    cout << "Fatal error: initialize '" << name << "' variable first before using\n";
    exit(1);
}

double ASTAssignmentNode::evaluate()
{
    if (trackSymbol.exists(left->name))
    {
        double val = right->evaluate();
        trackSymbol.set_initialized(left->name);
        symbolTable[left->name] = val;
        return val;
    }
    
    cout << "Fatal error: '" << left->name << "' is undefined\n";
    exit(1);
}

double ASTDeclarationNode::evaluate()
{
    return 0;
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

ASTNode* CreateDeclarationNode(const string& name)
{
    return new ASTDeclarationNode(name);
}

ASTNode* CreatePrintNode(ASTNode* expr)
{
    return new ASTPrintNode(expr);
}

ASTNode* CreateFunctionNode(vector<ASTNode*>& statements)
{
    return new ASTFunctionNode(statements);
}