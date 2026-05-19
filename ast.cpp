#include "ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ASTNode* CreateNode(NodeType type)
{
    ASTNode* node = new ASTNode;
    
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

ASTNode* CreateNumberNode(double val)
{
    ASTNode* node = CreateNode(NodeType::NODE_CONSTANT);
    node->data.val = val;
    return node;
}

ASTNode* CreateOpNode(char op, ASTNode* left, ASTNode* right)
{
    ASTNode* node = CreateNode(NodeType::NODE_BINARY_OP);
    node->data.op = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* CreateIdentifierNode(const string& name)
{
    ASTNode* node = CreateNode(NodeType::NODE_IDENTIFIER);
    strncpy(node->data.name, name.c_str(), 63);
    return node;
}