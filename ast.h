#pragma once

#include <string>
#include <unordered_map>

#include "builder.h"

extern std::unordered_map<std::string, double> symbolTable;

using namespace std;

enum class PrimitiveType
{
    BYTE, WORD, DWORD, QWORD
};

class Node
{
public:
    virtual ~Node() = default;
    virtual string build(Builder& builder) = 0;
};

class Constant  : public Node
{
public:
    double val;

    Constant(double value);
    
    string build(Builder& bulder) override;
};

class BinaryOp : public Node
{
public:
    char op;
    Node* left;
    Node* right;
    
    BinaryOp(char op, Node* left, Node* right);
    ~BinaryOp();

    string build(Builder& bulder) override;
};

class DeclareVariable : public Node
{
public:
    string name;
    Node* expr;
    DeclareVariable(string name, Node* expr = nullptr);
    ~DeclareVariable();
    string build(Builder& bulder) override;
};

class GetVariable : public Node
{
public:
    string name;
    
    GetVariable(string name);

    string build(Builder& bulder) override;
};

class Assignment : public Node
{
public:
    GetVariable* left;
    Node* right;

    Assignment(GetVariable* left, Node* right);
    ~Assignment();

    string build(Builder& bulder) override;
};



Node* CreateConstantNode(double val);
Node* CreateBinaryOpNode(char op, Node* left, Node* right);

Node* CreateDeclareVariableNode(string name);
Node* CreateDeclareVariableNode(string name, Node* expr);
Node* CreateGetVariableNode(string name);

Node* CreateAssignmentNode(GetVariable* left, Node* right);
