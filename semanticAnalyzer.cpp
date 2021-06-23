//
// Created by galkesten on 22/05/2021.
//

#include "symbolTables.h"
#include "semanticAnalyzer.h"
#include <stack>
#include "hw3_output.hpp"
#include <iostream>
using std::stack;

extern int yylineno;
symbolTables tables;
stack<long long> offsets;
stack<scope> scopes;
bool isMainDeclared = false;
int isWhile = 0;
int isSwitch = 0;
string curr_func_name = "";


scope::scope(scopeType type, long long table_index, string func_name,
             string ret_type) : scope_type(type), table_index(table_index), func_name(func_name), ret_type(ret_type){
}
void addPrintFuncitns(){
    vector<string> argTypes = {"STRING"};
    vector<string> argNames= {"input_str"};
    tables.pushNewFunc("print", "VOID", argTypes, argNames);
    tables.popTable();
    argTypes.clear();
    argTypes.push_back("INT");
    argNames.clear();
    argNames.push_back("num");
    tables.pushNewFunc("printi", "VOID",argTypes,argNames);
    tables.popTable();

}
void openNewScope(scopeType type, const string& name , const string&
ret_type , const vector<string>& argsTypes,
        const vector<string>& argNames) {


    long long table_index;
    if(type != FUNC_BLOCK){
        table_index = tables.pushNewTable();
    }
    else{
        try{
            table_index = tables.pushNewFunc(name, ret_type, argsTypes,
                    argNames);
            if(name == "main" && ret_type == "VOID" && argsTypes.size() == 0){
                isMainDeclared = true;
            }
            curr_func_name = name;
        } catch (symbolTables::symbolExist& e){
            output::errorDef(yylineno, e.name);
            exit(1);
        }

    }
    if(type == WHILE_BLOCK){
       isWhile++;
    }
    if(type == SWITCH_BLOCK)
        isSwitch++;
    scope new_scope(type, table_index, name, ret_type);

    scopes.push(new_scope);
    if(type == GLOBAL_BLOCK){
        offsets.push(-1);
        addPrintFuncitns();
    }
    else{
        offsets.push(offsets.top());
    }
}



void closeScope(){
//    output::endScope();
    scope& curr_scope = scopes.top();
    vector<symbolTableTuple>& curr_table = tables.getTable(curr_scope
            .table_index);
//    for(auto& symbol : curr_table){
//        string name = symbol.name;
//        long long offset = symbol.offset;
//        string type;
//        if(symbol.is_func){
//            type = output::makeFunctionType(symbol.type, symbol.argsTypes);
//        }
//        else{
//            type = symbol.type;
//        }
//        output::printID(name, offset, type);
//    }
    if(curr_scope.scope_type == WHILE_BLOCK)
        isWhile--;
    if(curr_scope.scope_type == SWITCH_BLOCK)
        isSwitch--;
    if(curr_scope.scope_type == FUNC_BLOCK){
        curr_func_name = "";
    }
    tables.popTable();
    scopes.pop();
    offsets.pop();
}

void addNewVar(const string& name, const string& type){

    ++offsets.top();
    try{
        tables.pushNewVar(name, type, offsets.top());
    } catch(symbolTables::symbolExist& e){

        output::errorDef(yylineno, e.name);
        exit(1);
    }
}

void checkIfVarDeclared(const string& name){
    if(!tables.isVarExist(name)){
        output::errorUndef(yylineno, name);
        exit(1);
    }
}

void checkFuncCall(const string& name, const vector<string>& inputTypes){
    funcErrorType error;
    if(!tables.isFuncExist(name, inputTypes, &error)){
        if(error == NOT_EXIST){
            output::errorUndefFunc(yylineno, name);
            exit(1);
        }
        if(error == TYPE_MISMATCH){
            output::errorPrototypeMismatch(yylineno, name, tables.getFuncArgs
            (name));
            exit(1);
        }

    }
}
/*
void checkTypes(const string& leftVarType, const string& rightVarType){

    if(leftVarType!=rightVarType && !(leftVarType=="INT" &&
    rightVarType=="BYTE")){
        output::errorMismatch(yylineno);
        exit(1);
    }

}
*/
void closeGlobalScope(){

    if(!isMainDeclared){
        output::errorMainMissing();
        exit(1);
    }

    closeScope();
}

string checkBoolType(const string& type1, const string& type2){
    if (type1!="BOOL" || type2!="BOOL"){
        output::errorMismatch(yylineno);
        exit(1);
    }
    return "BOOL";
}


string checkNumericType(const string& type1, const string& type2){
    if ((type1!="INT" && type1!="BYTE")||(type2!="INT" && type2!="BYTE")){
        output::errorMismatch(yylineno);
        exit(1);
    }

    if(type1 == "INT" || type2 == "INT")
        return "INT";
    else{
        return "BYTE";
    }
}


void checkAssignType(const string& type1, const string& type2){
    if (type1!=type2){
        if (type1 != "INT"||type2!="BYTE"){
            output::errorMismatch(yylineno);
            exit(1);
        }
    }
}


void checkByte(int val){
    if (val>255){
        output::errorByteTooLarge(yylineno, to_string(val));
        exit(1);
    }
}

void checkBreak(){
    if (isWhile == 0 && isSwitch == 0){
        output::errorUnexpectedBreak(yylineno);
        exit(1);
    }
}

void checkContinue(){
    if (isWhile == 0){
        output::errorUnexpectedContinue(yylineno);
        exit(1);
    }
}
void checkVoidFunc(){
    string ret_type = tables.getType(curr_func_name);
    if (ret_type != "VOID"){
        output::errorMismatch(yylineno);
        exit(1);
    }
}
void checkRetType(const string& type){
    string ret_type = tables.getType(curr_func_name);
    if ((ret_type == "VOID") || !((type == ret_type) || (type == "BYTE" && ret_type == "INT"))){
        output::errorMismatch(yylineno);
        exit(1);
    }
}

string getSymbolType(const string& name){

    return tables.getType(name);

}