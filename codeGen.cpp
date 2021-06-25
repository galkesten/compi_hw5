#include "codeGen.hpp"
#include "symbolTables.h"
#include <iostream>
extern symbolTables tables;
extern int isWhile;
extern int isSwitch;
extern stack<scopeType> scopeTypeStack;
typedef long long ll;

void printVector(const vector<pair<int,BranchLabelIndex>>& address_list){
    for(auto& elem : address_list){
        std::cout << "row : " << elem.first << " Label: " << elem.second << std::endl;
    }
}

codeGen &codeGen::instance(){
    static  codeGen inst;
    return inst;
}

string codeGen::newVar(){
    string var_name = "%var" + to_string(count_var);
    count_var++;
    return var_name;
}


void genBinopInstruction(semanticAttributes& dest, const semanticAttributes& num1,
                           const semanticAttributes& num2, const string& instruction) {
    string temp = codeGen::instance().newVar();
    dest.place = temp;
    if (instruction == "sdiv") {
        CodeBuffer::instance().emit("call void (i32) @error_zero(i32 " + num2.place + ")");

//        string dest_comp = codeGen::instance().newVar();
//        cmpInstruction cmp(num2.place, to_string(0), dest_comp, "i32", "eq");
//        cmp.emit();
//        conditionalBrInstruction br("%label_true", "%label_false", dest_comp);
//        br.emit();
//        CodeBuffer::instance().emit("%label_true:");
//        string string_error = codeGen::instance().newVar();
//        getElementPtrInstruction elem_prt(string_error,
//                "@.error_zero", "i8", 23, "0", "0");
//        elem_prt.emit();
//        string to_emit = "call void (i8*) @print(i8* " + string_error + ")";
//        CodeBuffer::instance().emit(to_emit);
//        CodeBuffer::instance().emit("call void (i32) @exit(i32 0)");
//        CodeBuffer::instance().emit("%label_false:");
    }
    binopInstruction bin(num1.place, num2.place, dest.place, "i32", instruction);
    bin.emit();
    if (dest.type == "BYTE") {
        string dest_trunc = codeGen::instance().newVar();
        string dest_zext = codeGen::instance().newVar();
        truncInstruction trunc_inst(dest.place, dest_trunc, "i32", "i8");
        trunc_inst.emit();
        zextInstruction zext_inst(dest_trunc, dest_zext, "i8", "i32");
        zext_inst.emit();
        dest.place = dest_zext;
    }
}


void genInt(semanticAttributes& attribute) {
    attribute.place = codeGen::instance().newVar();
    binopInstruction inst(to_string(attribute.intVal), to_string(0), attribute.place, "i32",
                          "add");
    inst.emit();
}

void genByte(semanticAttributes& attribute) {
    attribute.place = codeGen::instance().newVar();
    //cout<< "byte val" << (int)attribute.byteVal << endl;
    binopInstruction inst(to_string(attribute.byteVal), to_string(0), attribute.place, "i32",
                       "add");
    inst.emit();
}


void addPrintFunctionsToGlobalScope() {
    CodeBuffer::instance().emitGlobal("declare i32 @printf(i8*, ...)");
    CodeBuffer::instance().emitGlobal("declare void @exit(i32)");
    CodeBuffer::instance().emitGlobal("@.int_specifier = constant [4 x i8] c\"%d\\0A\\00\"");
    CodeBuffer::instance().emitGlobal("@.str_specifier = constant [4 x i8] c\"%s\\0A\\00\"");
    defineString error("\"Error division by zero\"", "@.error_zero");
    error.emit();
    CodeBuffer::instance().emitGlobal("define void @printi(i32) {");
    CodeBuffer::instance().emitGlobal("%spec_ptr = getelementptr [4 x i8], [4 x i8]* @"
                                      ".int_specifier, i32 0, i32 0");
    CodeBuffer::instance().emitGlobal("call i32 (i8*, ...) @printf(i8* %spec_ptr, i32 %0)");
    CodeBuffer::instance().emitGlobal("ret void");
    CodeBuffer::instance().emitGlobal("}");

    CodeBuffer::instance().emitGlobal("define void @print(i8*) {");
    CodeBuffer::instance().emitGlobal("%spec_ptr = getelementptr [4 x i8], [4 x i8]* @"
                                      ".str_specifier, i32 0, i32 0");
    CodeBuffer::instance().emitGlobal("call i32 (i8*, ...) @printf(i8* %spec_ptr, i8* %0)");
    CodeBuffer::instance().emitGlobal("ret void");
    CodeBuffer::instance().emitGlobal("}");
    CodeBuffer::instance().emitGlobal("define void @error_zero(i32) {");
    string dest_comp = codeGen::instance().newVar();
    cmpInstruction cmp("%0", to_string(0), dest_comp, "i32", "eq");
    cmp.emit(true);
    conditionalBrInstruction br("%label_true", "%label_false", dest_comp);
    br.emit(true);
    CodeBuffer::instance().emitGlobal("label_true:");
    string string_error = codeGen::instance().newVar();
    getElementPtrInstruction elem_prt(string_error,
            "@.error_zero", "i8", 23, "0", "0");
    elem_prt.emit(true);
    string to_emit = "call void (i8*) @print(i8* " + string_error + ")";
    CodeBuffer::instance().emitGlobal(to_emit);
    CodeBuffer::instance().emitGlobal("call void (i32) @exit(i32 0)");
    CodeBuffer::instance().emitGlobal("ret void");
    CodeBuffer::instance().emitGlobal("label_false:");
    CodeBuffer::instance().emitGlobal("ret void");
    CodeBuffer::instance().emitGlobal("}");

}

void printCode(){
    CodeBuffer::instance().printGlobalBuffer();
    CodeBuffer::instance().printCodeBuffer();
}

string codeGen::newStringVar() {
    static unsigned long long counter = 0;
    string new_str;
    new_str ="@.s" + to_string(counter);
    counter++;
    return new_str;
}
void genString(semanticAttributes& node) {

    string name = codeGen::instance().newStringVar();
    defineString new_str(node.stringVal, name);
    new_str.emit();
    string reg =codeGen::instance().newVar();
    getElementPtrInstruction getelem(reg, name, "i8", new_str.getLen(), "0", "0" );
    getelem.emit();
    node.place = reg;

}


void genBool(semanticAttributes& boolAttribute, bool boolVal){
    string val = "0";
    if(boolVal){
        val ="1";
    }
    conditionalBrInstruction br("@", "@", val);
    br.emit();
    int inst_loc = CodeBuffer::instance().getSize();
    boolAttribute.trueList.push_back(make_pair(inst_loc,FIRST));
    boolAttribute.falseList.push_back(make_pair(inst_loc,SECOND));
}

void genNotBool(semanticAttributes& dest, const semanticAttributes& src){
    dest.trueList=src.falseList;
    dest.falseList=src.trueList;
}

void genBoolOr(semanticAttributes& dest, const semanticAttributes& exp1, const semanticAttributes& m, const semanticAttributes& exp2){
    CodeBuffer::instance().bpatch(exp1.falseList, m.label);
    dest.trueList = CodeBuffer::merge(exp1.trueList, exp2.trueList);
    dest.falseList = exp2.falseList;
}

void genBoolAnd(semanticAttributes& dest, const semanticAttributes& exp1, const semanticAttributes& m, const semanticAttributes& exp2){
    CodeBuffer::instance().bpatch(exp1.trueList, m.label);
    dest.falseList=CodeBuffer::merge(exp1.falseList, exp2.falseList);
    dest.trueList = exp2.trueList;
}

void genRelational(semanticAttributes& dest, const semanticAttributes& exp1, const string& op, const semanticAttributes& exp2){
    string operation;
    string var = codeGen::instance().newVar();
    if (op == "==") operation = "eq";
    else if (op=="!=") operation="ne";
    else if (op=="<") operation="slt";
    else if (op=="<=") operation="sle";
    else if (op==">") operation="sgt";
    else if (op==">=") operation="sge";
    cmpInstruction cmp(exp1.place, exp2.place, var, "i32", operation);
    cmp.emit();
    conditionalBrInstruction br("@", "@", var);
    br.emit();
    int inst_loc = CodeBuffer::instance().getSize();
    dest.trueList.push_back(make_pair(inst_loc, FIRST));
    dest.falseList.push_back(make_pair(inst_loc, SECOND));
    dest.place = var;
}



void genMarker(semanticAttributes& m){
    string label=CodeBuffer::instance().genLabel();
    m.label=label;
}
void genLoad(semanticAttributes& dest, semanticAttributes& id){
    auto& gen_inst = codeGen::instance();
    ll offset = tables.getOffset(id.stringVal);
    if (offset < 0) { // this symbol is func arg;
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
void setReturn(bool val = false){
    auto& generator = codeGen::instance();
    generator.ReturnWasLastStatement = val;

}
void genFuncDefenition(const string& funcName){
    string retType = tables.getType(funcName);
    vector<string>& argTypes = tables.getFuncArgs(funcName);
    defineFuncInstruction func(argTypes,funcName, retType);
    func.emit();
    string place = codeGen::instance().newVar();
    allocateArrayInstruction allocate(place, "i32", 50);
    allocate.emit();
    codeGen::instance().curr_func_stack_pointer = place;
}


string genCodeForDecidingBoolExpVal(semanticAttributes& exp){
   auto& buffer = CodeBuffer::instance();
   auto& generator = codeGen::instance();
   string trueLabel = buffer.genLabel();
   string trueReg = generator.newVar();
   binopInstruction calcTrue("1", "0", trueReg, "i32", "add");
   calcTrue.emit();
   unconditionalBrInstruction br("@");
   br.emit();
   int add1 = buffer.getSize();
   string falseLabel = buffer.genLabel();
   string falseReg = generator.newVar();
   binopInstruction calcFalse("0", "0", falseReg, "i32", "add");
   calcFalse.emit();
   br.emit();
   int add2 = buffer.getSize();
   string finalLabel = buffer.genLabel();
   string finalReg =  generator.newVar();
   phiInstruction phi(trueLabel, falseLabel, trueReg, falseReg, finalReg, "i32");
   phi.emit();
   buffer.bpatch(buffer.makelist({add1, FIRST}), finalLabel);
   buffer.bpatch(buffer.makelist({add2, FIRST}), finalLabel);
   buffer.bpatch(exp.trueList, trueLabel);
   buffer.bpatch(exp.falseList, falseLabel);
   return finalReg;
}
void genIntReturn(semanticAttributes& exp){
    auto& buffer = CodeBuffer::instance();
    string res;
    if(exp.type == "BOOL"){
        res = genCodeForDecidingBoolExpVal(exp);
    }
    else{
        res = exp.place;
    }
    buffer.emit("ret i32 " + res);
}

void genVoidReturn(){
    auto& buffer = CodeBuffer::instance();
    buffer.emit("ret void");
}

void genCloseFunc(const string& retType){
    auto& buffer = CodeBuffer::instance();
    if(retType != "VOID"){
        buffer.emit("ret i32 0");
    }
    else{
        genVoidReturn();
    }

    buffer.emit("}");
}

string genFuncCall(const semanticAttributes& explist, const string& funcName ){
    string retType = tables.getType(funcName);
    string dest = "";
    if(retType != "VOID"){
        dest = codeGen::instance().newVar();
    }
    callInstruction call(explist.variablesTypes, explist.variablesPlaces, funcName, retType, dest);
    call.emit();
    return dest;
}

void genVoidFuncCall(const string& funcName ){
    string retType = tables.getType(funcName);
    string dest = "";

    callInstruction call(vector<string>(), vector<string>(), funcName, retType, dest);
    call.emit();
}

void genVarStore(const string& varName, semanticAttributes& exp, bool is_default) {
    auto& gen_inst = codeGen::instance();
    ll offset = tables.getOffset(varName);
    string varPointer = gen_inst.newVar();

    string src;
    if(!is_default){
        if(exp.type == "BOOL"){
            src = genCodeForDecidingBoolExpVal(exp);
        }
        else{
            src = exp.place;
        }
    }
    else{
        src = gen_inst.newVar();
        binopInstruction calcDefault("0", "0", src, "i32", "add");
        calcDefault.emit();
    }
    getElementPtrInstruction getelem(varPointer, gen_inst.curr_func_stack_pointer, "i32", 50,
                                     "0", to_string(offset));
    getelem.emit();

    storeInstruction store(varPointer, "i32", src);
    store.emit();
}

void genBoolValForFuncArg(semanticAttributes& exp){
    if(exp.type != "BOOL")
        return;
    exp.place = genCodeForDecidingBoolExpVal(exp);
}

void genBoolRetType(semanticAttributes& exp){
    auto& gen_inst = codeGen::instance();
    if(exp.type!= "BOOL")
        return;
    string boolVal = gen_inst.newVar();
    truncInstruction tr(exp.place, boolVal, "i32", "i1");
    tr.emit();
    conditionalBrInstruction br("@", "@", boolVal);
    br.emit();
    int address = CodeBuffer::instance().getSize();
    exp.trueList.push_back(make_pair(address,FIRST));
    exp.falseList.push_back(make_pair(address,SECOND));
}


void genBrForNextList(semanticAttributes& dest){
    unconditionalBrInstruction br("@");
    br.emit();
    int address = CodeBuffer::instance().getSize();
    dest.nextList.push_back(make_pair(address,FIRST));
}


void genIfNoElse(semanticAttributes& dest, semanticAttributes& ifExp, semanticAttributes&
    markerBeforeStatement){
    // before we entered this func we created with markerBeforeStatment a label for the if statement
    // so we know that we need to bpatch the ifExp trueList to the label of the marker
    // because we ended the ifExp we need to say it to jump to NextStamtement so we created
    // uncoditional branch to @ (we will fill it out later)
    // we also merged the falseList ti nextStatment because if the exp is false we want to jump
    // to the same place we will jump after the if.
    auto& buffer = CodeBuffer::instance();
    buffer.bpatch(ifExp.trueList, markerBeforeStatement.label);
    genBrForNextList(dest);
    dest.nextList = CodeBuffer::merge(dest.nextList, ifExp.falseList);
}


void genIfElse(semanticAttributes& dest, semanticAttributes& ifExp, semanticAttributes&
markerBeforeIfStatement, semanticAttributes& markerBeforeElseStatement, semanticAttributes& N){
    auto& buffer = CodeBuffer::instance();
    buffer.bpatch(ifExp.trueList, markerBeforeIfStatement.label);
    buffer.bpatch(ifExp.falseList, markerBeforeElseStatement.label);
    genBrForNextList(dest);//for the Else
    dest.nextList = CodeBuffer::merge(dest.nextList, N.nextList);
}

string createNextLabelAndBpatchNextList(semanticAttributes& statement){
    auto& buffer = CodeBuffer::instance();
    if(statement.nextList.empty())
        return "";
    string nextLabel =buffer.genLabel();
    buffer.bpatch(statement.nextList, nextLabel);
    statement.nextList.clear();
    return nextLabel;

}
void mergeNextLists(semanticAttributes& dest, semanticAttributes& src1, semanticAttributes& src2){
    auto& buffer = CodeBuffer::instance();
    dest.nextList = buffer.merge(src1.nextList, src2.nextList);
}


string genBrToWhileOrSwitchStartLabel(){
    auto& buffer = CodeBuffer::instance();
    unconditionalBrInstruction br("@");
    br.emit();
    int address = buffer.getSize();
    string label = buffer.genLabel();
    auto list = buffer.makelist({address, FIRST});
    buffer.bpatch(list, label);
    return label;
}
whileInfo::whileInfo(const string &label) : label(label), nextList(vector<pair<int,BranchLabelIndex>>()){}

 void genLabelForWhileOrSwitch(bool whileScope){
    string entryLabel =  genBrToWhileOrSwitchStartLabel();
    if(whileScope){
        struct whileInfo info( entryLabel);
        codeGen::instance().whileInfoStack.push(info);
    }
    else{
        struct switchInfo info(entryLabel);
        codeGen::instance().switchInfoStack.push(info);
    }
}

void genWhile(semanticAttributes& dest, semanticAttributes& whileExp, semanticAttributes&
markerBeforeStatement){

    auto& buffer = CodeBuffer::instance();
    auto& generator = codeGen::instance();
    buffer.bpatch(whileExp.trueList, markerBeforeStatement.label);
    struct whileInfo curr = generator.whileInfoStack.top();
    generator.whileInfoStack.pop();
    unconditionalBrInstruction brBackToWhile(curr.label);
    brBackToWhile.emit();
    dest.nextList = CodeBuffer::merge(dest.nextList, whileExp.falseList);
    dest.nextList = CodeBuffer::merge(dest.nextList, curr.nextList);
}

void genBreak(){
    auto& buffer = CodeBuffer::instance();
    auto& generator = codeGen::instance();
    unconditionalBrInstruction br("@");
    br.emit();
    int address = buffer.getSize();
    if(scopeTypeStack.top()==WHILE_BLOCK){
        auto& curr = generator.whileInfoStack.top();
        curr.nextList.push_back({address, FIRST});
    }
    else{
        auto& curr = generator.switchInfoStack.top();
        curr.breakNextList.push_back({address, FIRST});
    }

}

void genContinue(){
    auto& generator = codeGen::instance();
    auto& curr = generator.whileInfoStack.top();
    unconditionalBrInstruction br(curr.label);
    br.emit();
}


switchInfo::switchInfo(const string &label) : label(label), breakNextList(vector<pair<int,
        BranchLabelIndex>>()), jumpToStartCaseList(vector<pair<int,
        BranchLabelIndex>>()), caseList(vector<pair<string,string>>()){}

void genNewCase(semanticAttributes& num, bool isDefault){
    string caseLabel = genBrToWhileOrSwitchStartLabel();
    auto& curr = codeGen::instance().switchInfoStack.top();
    string caseVal = "DEFAULT";
    if(!isDefault){
        caseVal  = to_string(num.intVal);
    }
    curr.caseList.push_back({caseLabel, caseVal});
}

void genBrToCaseList(){
    auto& buffer = CodeBuffer::instance();
    auto& generator = codeGen::instance();
    unconditionalBrInstruction br("@");
    br.emit();
    int address = buffer.getSize();
    auto& curr = generator.switchInfoStack.top();
    curr.jumpToStartCaseList.push_back({address, FIRST});
}

void genSwitch(semanticAttributes& exp){
    auto& buffer = CodeBuffer::instance();
    auto& generator = codeGen::instance();
    auto& curr = generator.switchInfoStack.top();
    unconditionalBrInstruction br("@");
    br.emit();
    int address = buffer.getSize();
    auto nextList = buffer.makelist({address, FIRST});
    string startCaseList = buffer.genLabel();
    buffer.bpatch(curr.jumpToStartCaseList, startCaseList);

    string currLabel;
    for(auto& caseElem : curr.caseList){
        string cmpRes = generator.newVar();
        string src1 = exp.place;
        string src2 = caseElem.second =="DEFAULT" ?  src1 :  caseElem.second;
        cmpInstruction cmp(src1, src2, cmpRes,"i32", "eq");
        cmp.emit();
        conditionalBrInstruction br(caseElem.first, "@", cmpRes);
        br.emit();
        int address = buffer.getSize();
        currLabel = buffer.genLabel();
        buffer.bpatch(buffer.makelist({address, SECOND}), currLabel);
    }
    if(scopeTypeStack.top()==SWITCH_BLOCK) {
        nextList = buffer.merge(nextList, curr.breakNextList);
        buffer.bpatch(nextList, currLabel);
    }
    else{
        auto& currWhile = generator.whileInfoStack.top();
        buffer.bpatch(curr.breakNextList, currWhile.label);
        buffer.bpatch(nextList, currLabel);
    }

}