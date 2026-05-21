#include "ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ASTNode::ASTNode(ASTNodeType type)
{
    this->type = type;
    this->left = NULL;
    this->right = NULL;
}

ASTNode::~ASTNode()
{
    delete left;
    delete right;
}

ASTNode* CreateNumberNode(double val)
{
    ASTNode* node = new ASTNode(ASTNodeType::NODE_CONSTANT);
    node->data.val = val;
    return node;
}

ASTNode* CreateOpNode(char op, ASTNode* left, ASTNode* right)
{
    ASTNode* node = new ASTNode(ASTNodeType::NODE_BINARY_OP);
    node->data.op = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* CreateIdentifierNode(const string& name)
{
    ASTNode* node = new ASTNode(ASTNodeType::NODE_IDENTIFIER);
    strncpy(node->data.name, name.c_str(), 63);
    return node;
}