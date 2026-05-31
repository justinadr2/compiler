#pragma once

#include <string>
#include <unordered_map>

extern std::unordered_map<std::string, double> symbolTable;

using namespace std;

enum class PrimitiveType
{
    BYTE, WORD, DWORD, QWORD
};

class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual double evaluate() = 0;
};

class ASTConstantNode : public ASTNode
{
public:
    double val;

    ASTConstantNode(double value);
    
    double evaluate() override;
};

class ASTOpNode : public ASTNode
{
public:
    char op;
    ASTNode* left;
    ASTNode* right;
    
    ASTOpNode(char op, ASTNode* left, ASTNode* right);
    ~ASTOpNode();

    double evaluate() override;
};

class ASTVariableNode : public ASTNode
{
public:
    string name;
    bool initialized;
    
    ASTVariableNode(const string& name);

    double evaluate() override;
};

class ASTAssignmentNode : public ASTNode
{
public:
    ASTVariableNode* left;
    ASTNode* right;

    ASTAssignmentNode(ASTVariableNode* left, ASTNode* right);
    ~ASTAssignmentNode();

    double evaluate() override;
};

class ASTPrintNode : public ASTNode
{
public:
    ASTNode* expr;
    
    ASTPrintNode(ASTNode* expr);
    ~ASTPrintNode();
    
    double evaluate() override;
};

class ASTFunctionNode : public ASTNode
{
public:
    vector<ASTNode*> statements;
    ASTFunctionNode(vector<ASTNode*>& statements);

    double evaluate() override;
    ~ASTFunctionNode();
};

ASTNode* CreateConstantNode(double val);
ASTNode* CreateOpNode(char op, ASTNode* left, ASTNode* right);
ASTNode* CreateVariableNode(const string& name);
ASTNode* CreateAssignmentNode(ASTVariableNode* left, ASTNode* right);
ASTNode* CreatePrintNode(ASTNode* expr);
ASTNode* CreateFunctionNode(vector<ASTNode*>& statements);