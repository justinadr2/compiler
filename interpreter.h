#pragma once

#include "ast.h"
#include <unordered_map>
#include <string>

using namespace std;

extern unordered_map<string, double> symbolTable;

double Evaluate(ASTNode* node);
