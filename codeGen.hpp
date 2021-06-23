
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


void genBinopInstruction(semanticAttributes& dest, const semanticAttributes& num1,
                         const semanticAttributes& num2, const string& instruction);

void genInt(semanticAttributes& attribute);
void genByte(semanticAttributes& attribute);
void genString(semanticAttributes& attribute);
void addPrintFunctionsToGlobalScope();
void printCode();
