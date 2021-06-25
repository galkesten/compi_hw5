//
// Created by galkesten on 22/05/2021.
//

#include "symbolTables.h"
#include "hw3_output.hpp"
#include <iostream>

symbolTableTuple::symbolTableTuple(const string &name, const string &type,
                                   long long offset, bool is_func,
                                   const vector<string> &argTypes,
                                   const vector<string> &argNames) : name(name), type(type), offset(offset), is_func(is_func), argsTypes(argTypes), argNames(argNames){}


symbolTables::symbolTables() {

    tables = vector<vector<symbolTableTuple>>();
}

symbolTables::symbolExist::symbolExist(const string &name, bool is_func) : name
(name), is_func(is_func) {}


long long symbolTables::pushNewTable() {

    tables.emplace_back(vector<symbolTableTuple>());

    return tables.size()-1;

}

void symbolTables::popTable() {

    tables.pop_back();
}

int lookForString(const string& name, vector<symbolTableTuple>& vec){
    for(unsigned int i =0; i < vec.size(); ++i){
        if (vec[i].name == name)
            return i;
    }
    return -1;
}
bool symbolTables::isVarExist(const string &name) {
    long long i = tables.size()-1;
    while(i>=0){
        int res = lookForString(name, tables[i]);
        if(res > -1){
            symbolTableTuple& var = tables[i][res];
            if(!var.is_func)
                return true;
        }
        --i;
    }
    return false;
}


bool symbolTables::isSymbolExist(const string &name) {
    long long i = tables.size()-1;
    while(i>=0){
        int res = lookForString(name, tables[i]);
        if(res > -1){
            return true;
        }
        --i;
    }
    return false;
}
bool isFuncTypeMatching(const vector<string>& funcDeclArgTypes, const
    vector<string>& callArgTypes ){

    if(funcDeclArgTypes.size() != callArgTypes.size())
        return false;

    long long size = funcDeclArgTypes.size();
    for(int i=0; i< size; i++){
        if(funcDeclArgTypes[i]!= callArgTypes[i] &&
                !(funcDeclArgTypes[i]=="INT" && callArgTypes[i]=="BYTE")){
            return false;
        }
    }
    return true;

}
bool symbolTables::isFuncExist(const string &name, const vector<string>&
        callArgTypes, funcErrorType* error) {
    long long i = tables.size()-1;
    while(i>=0){
        int res = lookForString(name, tables[i]);
        if(res > -1){
            symbolTableTuple func = tables[i][res];
            if(func.is_func){
                if(isFuncTypeMatching(func.argsTypes,callArgTypes ))
                    return true;
                else{
                    *error= TYPE_MISMATCH;
                    return false;
                }

            }

        }
        --i;
    }
    *error = NOT_EXIST;
    return false;
}


void symbolTables::pushNewVar(const string &name, const string &type,
                              long long offset) {

    if(this->isSymbolExist(name)){
        throw symbolExist(name, false);
    }
    symbolTableTuple new_tuple(name, type, offset, false, vector<string>(), vector<string>());
    //cout << "name in pushNewVar :" <<  name << endl;
    //cout << "offset in pushNewVar :" << offset << endl;
    tables[tables.size()-1].push_back(new_tuple);
}

long long symbolTables::pushNewFunc(const string &name, const string &ret_type,
        const vector<string> &argsTypes, const vector<string> &argNames) {

    if(this->isSymbolExist(name)){
        throw symbolExist(name, true);
    }
    symbolTableTuple new_tuple(name, ret_type, 0, true, argsTypes, argNames);

    tables[tables.size()-1].push_back(new_tuple);
    int table_index = this->pushNewTable(); //creating new table for functions;
    for(long long i=0; i<argsTypes.size(); ++i){
       // cout << "i is " << i << endl;
        long long offset = 0;
        offset = -i-1;
       // cout << "offset in pushNewFunc :" << offset << endl;
        this->pushNewVar(argNames[i], argsTypes[i], offset);
    }

    return table_index;

}

vector<symbolTableTuple>& symbolTables::getTable(long long index) {

    return tables[index];
}

string symbolTables::getType(const string &symbol) {

    long long i = tables.size()-1;
    string type;
    while(i>=0){
        int res = lookForString(symbol, tables[i]);
        if(res > -1){
           type = tables[i][res].type;
           break;

        }
        --i;
    }
    return type;

}

long long symbolTables::getOffset(const string &symbol) {

    long long i = tables.size()-1;
    long long offset = -1;
    while(i>=0){
        int res = lookForString(symbol, tables[i]);
        if(res > -1){
           // cout << "res: " << res << endl;
            offset = tables[i][res].offset;
           // cout << "offset :" << offset << endl;
            break;

        }
        --i;
    }

    return offset;

}


vector<string>& symbolTables::getFuncArgs(const string &name) {
    long long i = tables.size()-1;
    while(i>=0){
        int res = lookForString(name, tables[i]);
        if(res > -1){
            return tables[i][res].argsTypes;
        }
        --i;
    }
    exit(1);
}