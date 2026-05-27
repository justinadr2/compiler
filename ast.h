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
    char operation;
    ASTNode* left;
    ASTNode* right;
    
    ASTOpNode(char operation, ASTNode* left, ASTNode* right);
    ~ASTOpNode() override;
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
    ~ASTAssignmentNode() override;
    double evaluate() override;
};

class ASTPrintNode : public ASTNode
{
public:
    ASTNode* expr;
    
    ASTPrintNode(ASTNode* expression);
    ~ASTPrintNode() override;
    
    double evaluate() override;
};

ASTNode* CreateConstantNode(double val);
ASTNode* CreateOpNode(char op, ASTNode* left, ASTNode* right);
ASTNode* CreateVariableNode(const string& name);
ASTNode* CreateAssignmentNode(ASTVariableNode* left, ASTNode* right);