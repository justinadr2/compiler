#pragma once

#include <stddef.h>
#include <string>

using namespace std;

enum class ASTNodeType {
    NODE_CONSTANT,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,
    NODE_ASSIGNEMENT
};

class ASTNode
{
public:
    ASTNodeType type;
    union {
        double val;
        char name[32];
        char op;
    } data;

    ASTNode* left;
    ASTNode* right;

    ASTNode() {}

    ASTNode(ASTNodeType type);

    ~ASTNode();
};

ASTNode* CreateNumberNode(double val);
ASTNode* CreateOpNode(char op, ASTNode* left, ASTNode* right);
ASTNode* CreateIdentifierNode(const string& name);
