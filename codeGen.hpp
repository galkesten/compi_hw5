
#include "Instructions.h"
#include "semanticAnalyzer.h"
#include "bp.hpp"

class codeGen{
    int count_var=0;
    codeGen() = default;
public:

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
