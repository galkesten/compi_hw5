//
// Created by galkesten on 23/06/2021.
//

#include "semanticAnalyzer.h"
#include "codeGen.hpp"

void genLoad(semanticAttributes& dest, semanticAttributes& , long long offset){
    auto& gen_inst = codeGen::instance();
    //ll offset = tables.getOffset(id.stringVal);
    if (offset == -1) { // this symbol is func arg;
        dest.place= "%" + to_string(-(offset+1));
    }
    else{
        string varPointer = gen_inst.newVar();
        getElementPtrInstruction getelem(varPointer, gen_inst.curr_func_stack_pointer, "i32", 50,
                                         "0", to_string(offset));
        getelem.emit();
        string res = gen_inst.newVar();
        loadInstruction load(varPointer, res, "i32");
        load.emit();
        dest.place = res;
    }
    if(dest.type == "BOOL"){
        string boolVal = gen_inst.newVar();
        truncInstruction tr(dest.place, boolVal, "i32", "i1");
        tr.emit();
        conditionalBrInstruction br("@", "@", boolVal);
        br.emit();
        int address = CodeBuffer::instance().getSize();
        dest.trueList.push_back(make_pair(address,FIRST));
        dest.falseList.push_back(make_pair(address,SECOND));
    }
}
int main(){

    semanticAttributes node1;
    node1.stringVal = "x";
    semanticAttributes node2;
    node2.type = "BOOL";
    genLoad(node2, node1, 2);
    CodeBuffer::instance().printCodeBuffer();


}