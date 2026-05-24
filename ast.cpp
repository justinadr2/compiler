#include "ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ASTNode::ASTNode(ASTNodeType type) :
    type(type), left(nullptr), right(nullptr) {}

ASTNode::ASTNode(ASTNodeType type, ASTNode* left, ASTNode* right) :
    type(type), left(left), right(right) {}

ASTNode::~ASTNode()
{
    delete left;
    delete right;
}

ASTNode* CreateNumberNode(double val)
{
    ASTNode* node = new ASTNode(ASTNodeType::CONSTANT);
    node->data.val = val;
    return node;
}

ASTNode* CreateOpNode(char op, ASTNode* left, ASTNode* right)
{
    ASTNode* node = new ASTNode(ASTNodeType::BINARY_OP, left, right);
    node->data.op = op;
    return node;
}

ASTNode* CreateIdentifierNode(const string& name)
{
    ASTNode* node = new ASTNode(ASTNodeType::IDENTIFIER);
    strncpy(node->data.name, name.c_str(), 63);
    return node;
}