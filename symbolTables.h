//
// Created by galkesten on 21/05/2021.
//

#ifndef COMPI_HW3_TYPES_H
#define COMPI_HW3_TYPES_H

#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;


struct symbolTableTuple{
    string name;
    string type; // for func - type its return type
    long long offset;
    bool is_func;
    vector<string> argsTypes;
    vector<string> argNames;

    symbolTableTuple() = default;
    symbolTableTuple(const string& name, const string& type, long long offset, bool is_func, const vector<string>& argTypes, const vector<string>& argNames);
};

enum funcErrorType{
    NOT_EXIST,
    TYPE_MISMATCH
};

class symbolTables {
    vector<vector<symbolTableTuple>> tables;

public:
    symbolTables();

    symbolTables(const symbolTables &) = default;

    ~symbolTables() = default;

    symbolTables& operator=(const symbolTables&) = default;

    long long pushNewTable();

    void popTable();
    void pushNewVar(const string& name, const string& type, long long offset);
    long long pushNewFunc(const string& name, const string& ret_type, const
    vector<string>& argsTypes=vector<string>(), const vector<string>&
            argNames=vector<string>());

    bool isVarExist(const string& name);

    bool isFuncExist(const string& name, const vector<string>& argTypes,
            funcErrorType* error);

    bool isSymbolExist(const string& name);

    string getType(const string& symbol);
    vector<string>& getFuncArgs(const string& name);
    vector<symbolTableTuple>& getTable(long long index);

    class symbolExist : public std::exception{

    public:
       string name;
       bool is_func;
       symbolExist(const string& name, bool is_func);
    };

    };
#endif //COMPI_HW3_TYPES_H
