#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include <unordered_map>
#include <string>

extern std::unordered_map<std::string, double> symbolTable;

double Evaluate(ASTNode* node);

#endif