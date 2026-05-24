#include "ast.h"
#include "interpreter.h"

#include <iostream>
#include <string>
#include <unordered_map>

unordered_map<string, double> symbolTable;

double Evaluate(ASTNode* node)
{
    if (!node)
        return 0.0;

    if (node->type == ASTNodeType::CONSTANT)
        return node->data.val;

    if (node->type == ASTNodeType::IDENTIFIER)
    {
        string varname(node->data.name);
        if (symbolTable.find(varname) == symbolTable.end())
        {
            cout << "Fatal error: Variable " << node->data.name << " is undefiend\n";
            exit(1);
        }
        return symbolTable[varname];
    }

    if (node->type == ASTNodeType::ASSIGNEMENT)
    {
        double val = Evaluate(node->right);
        string varname(node->left->data.name);
        symbolTable[varname] = val;
        return val;
    }

    if (node->type == ASTNodeType::BINARY_OP)
    {
        double left_val = Evaluate(node->left);
        double right_val = Evaluate(node->right);

        switch (node->data.op)
        {
            case '+': return left_val + right_val;
            case '-': return left_val - right_val;
            case '*': return left_val * right_val;
            case '/':
                if (right_val == 0)
                {
                    cout << "Fatal error: division by zero\n";
                    return 1;
                }
                return left_val / right_val;
            default:
                cout << "Unknown operator: " << node->data.op << '\n';
                return 1;
        }
    }

    return 1;
}