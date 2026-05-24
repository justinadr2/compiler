#pragma once

#include <stddef.h>
#include <string>

using namespace std;

enum class ASTNodeType {
    CONSTANT,
    IDENTIFIER,
    BINARY_OP,
    ASSIGNEMENT
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

    ASTNode(ASTNodeType type);
    ASTNode(ASTNodeType type, ASTNode* left, ASTNode* right);

    ~ASTNode();
};

ASTNode* CreateNumberNode(double val);
ASTNode* CreateOpNode(char op, ASTNode* left, ASTNode* right);
ASTNode* CreateIdentifierNode(const string& name);
