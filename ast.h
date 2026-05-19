#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <string>

using namespace std;

enum class NodeType {
    NODE_CONSTANT,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,
    NODE_ASSIGNEMENT
};

class ASTNode
{
public:
    NodeType type;
    union {
        double val;
        char name[64];
        char op;
    } data;

    ASTNode* left;
    ASTNode* right;

    ~ASTNode()
    {
        delete left;
        delete right;
    }
};

ASTNode* CreateNode(NodeType type);

ASTNode* CreateNumberNode(double val);
ASTNode* CreateOpNode(char op, ASTNode* left, ASTNode* right);
ASTNode* CreateIdentifierNode(const string& name);

#endif
