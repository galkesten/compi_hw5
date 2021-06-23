//
// Created by galkesten on 22/05/2021.
//

#ifndef COMPI_HW3_SEMANTICANALYZER_H
#define COMPI_HW3_SEMANTICANALYZER_H
#include "hw3_output.hpp"
#include "bp.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;


struct semanticAttributes{
    int intVal;
    char byteVal;
    string stringVal;
    string type;
    std::vector<string> variablesValues;
    std::vector<string> variablesTypes;
    string place;
    vector<string> varPlaces;
    string label;
    vector<pair<int,BranchLabelIndex>> trueList;
    vector<pair<int,BranchLabelIndex>> falseList;
    vector<pair<int,BranchLabelIndex>> nextList;
};

#define YYSTYPE semanticAttributes

enum scopeType {
    IF_BLOCK,
    ELSE_BLOCK,
    WHILE_BLOCK,
    FUNC_BLOCK,
    MAIN_BLOCK,
    SWITCH_BLOCK,
    BLOCK,
    GLOBAL_BLOCK
};


struct scope{
    scopeType scope_type;
    long long table_index;
    string func_name;
    string ret_type;

    scope(scopeType type, long long table_index, string func_name, string ret_type);
};




string checkBoolType(const string& type1, const string& type2);
void openNewScope(scopeType type, const string& name ="", const string&
ret_type="", const vector<string>& argsTypes=vector<string>(),
                  const vector<string>& argNames=vector<string>());
void closeScope();
void addNewVar(const string& name, const string& type);
void checkIfVarDeclared(const string& name);
void checkFuncCall(const string& name, const vector<string>& inputTypes= vector<string>());
void closeGlobalScope();
string checkNumericType(const string& type1, const string& type2);
void checkAssignType(const string& type1, const string& type2);
void checkByte(int val);
void checkBreak();
void checkContinue();
void checkRetType(const string& type);
void checkVoidFunc();
string getSymbolType(const string& name);
#endif //COMPI_HW3_SEMANTICANALYZER_H
