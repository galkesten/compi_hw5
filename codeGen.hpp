
#include "Instructions.h"
#include "semanticAnalyzer.h"
#include "bp.hpp"
#include <stack>

using std::stack;

struct whileInfo{
    string label;
    vector<pair<int,BranchLabelIndex>> nextList;

    whileInfo(const string& label);
};

struct switchInfo{
    string label;
    vector<pair<int,BranchLabelIndex>> breakNextList;
    vector<pair<int,BranchLabelIndex>> jumpToStartCaseList;
    vector<pair<string,string>> caseList;
    switchInfo(const string& label);
};

class codeGen{
    int count_var=0;

    codeGen() = default;
public:
    stack<whileInfo> whileInfoStack;
    stack<switchInfo> switchInfoStack;
    string curr_func_stack_pointer;
    bool ReturnWasLastStatement = false;
    static codeGen& instance();
    string newVar();
    string newStringVar();


};


void genBinopInstruction(semanticAttributes& dest, const semanticAttributes& num1,
                         const semanticAttributes& num2, const string& instruction);

void genInt(semanticAttributes& attribute);
void genByte(semanticAttributes& attribute);
void genString(semanticAttributes& attribute);
void addPrintFunctionsToGlobalScope();
void printCode();
void genBool(semanticAttributes& boolAttribute, bool boolVal);
void genNotBool(semanticAttributes& dest, const semanticAttributes& src);
void genBoolOr(semanticAttributes& dest, const semanticAttributes& exp1, const semanticAttributes& m, const semanticAttributes& exp2);
void genBoolAnd(semanticAttributes& dest, const semanticAttributes& exp1, const semanticAttributes& m, const semanticAttributes& exp2);
void genRelational(semanticAttributes& dest, const semanticAttributes& exp1, const string& op, const semanticAttributes& exp2);
void genMarker(semanticAttributes& m);
void genLoad(semanticAttributes& dest, semanticAttributes& id);
void printVector(const vector<pair<int,BranchLabelIndex>>& address_list);
void genFuncDefenition(const string& funcName);
void genIntReturn(semanticAttributes& exp);
void genVoidReturn();
void genCloseFunc(const string& retType);
string genFuncCall(const semanticAttributes& explist, const string& funcName );
void genVoidFuncCall(const string& funcName );
void genVarStore(const string& varName, semanticAttributes& exp, bool is_default = false);
void genBoolValForFuncArg(semanticAttributes& exp);
void genBoolRetType(semanticAttributes& exp);
void genIfNoElse(semanticAttributes& dest, semanticAttributes& ifExp, semanticAttributes&
markerBeforeStatement);
void genIfElse(semanticAttributes& dest, semanticAttributes& ifExp, semanticAttributes&
markerBeforeIfStatement, semanticAttributes& markerBeforeElseStatement, semanticAttributes& N);
string createNextLabelAndBpatchNextList(semanticAttributes& statement);
void mergeNextLists(semanticAttributes& dest, semanticAttributes& src1, semanticAttributes& src2);
void genBrForNextList(semanticAttributes& dest);
void genLabelForWhileOrSwitch(bool isWhile);
void genWhile(semanticAttributes& dest, semanticAttributes& whileExp, semanticAttributes&
markerBeforeStatement);
void genContinue();
void genBreak();
void genNewCase(semanticAttributes& num, bool isDefault);
void genBrToCaseList();
void genSwitch(semanticAttributes& exp);