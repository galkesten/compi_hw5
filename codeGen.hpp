
#include "Instructions.h"
#include "semanticAnalyzer.h"

class codeGen{
    int count_var=0;
    codeGen() = default;
public:

    static codeGen& instance();
    string newVar();
    string newStringVar();

};


void genBinopInstruction(const semanticAttributes& dest, const semanticAttributes& num1,
                         const semanticAttributes& num2, const string& instruction);

void genNum(const semanticAttributes& attribute);
void genByte(const semanticAttributes& attribute);
void genString(const semanticAttributes& attribute);
void addPrintFunctionsToGlobalScope();
void printCode();