//
// Created by galkesten on 19/06/2021.
//

#include "Instructions.h"

string addPercent(const string& s){
    if(s[0]== '@')
        return;
    if (s[0]!='%'){
        return "%"+s;
    }
    return s;
}

binopInstruction::binopInstruction(const string &src1, const string &src2,
                                   const string &dest, const string &type,
                                   const string& instruction): src1(src1),
                                   src2(src2), dest(dest), type(type), instruction(instruction){};

void binopInstruction::emit(bool is_global){
    string inst = dest + " = " + instruction + " " + type+ " " + src1+", " + src2;
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}


zextInstruction::zextInstruction(const string &src, const string &dest,
                                 const string type_src,
                                 const string &type_dest): src(src), dest(dest),
                                 type_src(type_src), type_dest(type_dest){};

void zextInstruction::emit(bool is_global){
    string inst = dest + "= zext " + type_src + " " + src + " to " + type_dest;
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}

truncInstruction::truncInstruction(const string &src, const string &dest,
                                   const string type_src,
                                   const string &type_dest): src(src), dest(dest),
                                   type_src(type_src), type_dest(type_dest){};

void truncInstruction::emit(bool is_global){
    string inst = dest + "=trunc " + type_src + " " + src + " to " + type_dest;
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}


allocateVarInstruction::allocateVarInstruction(const string& ptrName, const
string& type) : ptrName(ptrName){}

void allocateVarInstruction::emit(bool is_global) {
    string inst = ptrName + " = alloca "+type;
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}


allocateArrayInstruction::allocateArrayInstruction(const string& ptrName,
        const string& type, long long size) : ptrName(ptrName), type(type), size(size){}

void allocateArrayInstruction::emit(bool is_global)  {
    string inst = ptrName + " = alloca "+ type+ ", "+ type + " "+
    to_string(size);
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}


getElementPtrInstruction::getElementPtrInstruction(const string& destPtr,
        const string& srcPtr, const string& elemType, long long arraySize,
        const string& targetArrayIndex, const string& targetElementIndex) :
        Instruction(), destPtr(destPtr), srcPtr(srcPtr), elemType(elemType),
        arraySize(arraySize), targetArrayIndex(targetArrayIndex), targetElementIndex(targetElementIndex){}


void getElementPtrInstruction::emit(bool is_global) {
    string arrayType;
    if (arraySize > 1){
        arrayType= "[" + to_string(arraySize)+" x "+ elemType+ "]";
    }
    else{
        arrayType= elemType;
    }
    string inst = destPtr + " = getelementptr " + arrayType+ ", " +
            arrayType
            +"* "+
            srcPtr+", ";
    if(arraySize > 1){
        inst+= "i32 " +targetArrayIndex+ ", ";
    }
    inst+= "i32 " + targetElementIndex;
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}

storeInstruction::storeInstruction(const string& ptrName, const string& type,
        const string& val) : ptrName(ptrName), type(type), val(val){}


void storeInstruction::emit(bool is_global){
    string inst = "store " + type + " " + val+", "+ type + "*" + " "+ ptrName;
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}



loadInstruction::loadInstruction(const string& srcPtr,const string&
    destPtr, const string& type) : srcPtr(srcPtr), destPtr(destPtr), type(type){}

void loadInstruction::emit(bool is_global){
    string inst = destPtr + " = load " + type + ", "+ type +"* "+ srcPtr;
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}

cmpInstruction::cmpInstruction(const string& src1, const string& src2, const
string& dest, const string& type, const string& cond) :src1(src1),src2(src2),
dest(dest), type(type), cond(cond){}


void cmpInstruction::emit(bool is_global){
    string inst = dest + " = icmp " + cond + " "+ type+ " " + src1+", " +
            src2;
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}


unconditionalBrInstruction::unconditionalBrInstruction(const string& label) :
label(addPercent(label)){}

void unconditionalBrInstruction::emit(bool is_global){
    string inst = "br label "+label;
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}

conditionalBrInstruction::conditionalBrInstruction(const string& trueLabel,
        const string&
    falseLabel, const string& regOfCmpRes) : trueLabel(addPercent(trueLabel)),
    falseLabel(addPercent(falseLabel)), regOfCmpRes(regOfCmpRes){}

void conditionalBrInstruction::emit(bool is_global) {
    string inst = "br i1 " + regOfCmpRes+", label "+trueLabel+", label "
                                                             ""+falseLabel;
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}



string callInstruction::createParamsString(){
    string params="";
    for(int i=0; i< types.size(); ++i){
        params+= types[i];
        params+= " ";
        params+= places[i];

        if(i!=types.size()-1){
            params+=", ";
        }
    }
    return params;
    }

callInstruction::callInstruction(const vector<string>& types, const
                        vector<string>& places, const string& funcName,
                        const string& retType, const string& dest) {
    this->funcName = "@"+ funcName;
    this->retType = (retType=="VOID"? "void" : "i32");
    this->dest = dest;
    this->places = places;
    for(int i=0; i<types.size(); i++) {
        if(types[i] == "STRING"){
            this->types.push_back("i8*");
        }
        else{
            this->types.push_back("i32");
        }
    }

}

void callInstruction::emit(bool is_global){
    string inst;
    if(dest != ""){
        inst+= dest+ " = ";
    }
    string params = createParamsString();
    inst+= "call "+ retType +" "+ funcName+"("+params+")";
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}




string defineFuncInstruction::createParamsString(){
    string params="";
    for(int i=0; i< types.size(); ++i){
        params+= types[i];
        if(i!=types.size()-1){
            params+=", ";
        }
    }
    return params;
}

defineFuncInstruction::defineFuncInstruction(const vector<string>& types,const
string& funcName, const string& retType) {
    this->funcName = "@"+ funcName;
    this->retType = (retType=="VOID"? "void" : "i32");
    for(int i=0; i<types.size(); i++) {
        if(types[i] == "STRING"){
            this->types.push_back("i8*");
        }
        else{
            this->types.push_back("i32");
        }
    }

}

void defineFuncInstruction::emit(bool is_global){
    string params = createParamsString();
    string inst = "define "+ retType+ " "+funcName +"("+params+")"+" {";
    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}


phiInstruction::phiInstruction(const string &label1, const string &label2,
                               const string &val1, const string &val2,
                               const string &dest, const string& type) :
                               label1(addPercent(label1)),
                               label2(addPercent(label2)), val1(val1), val2(val2),
                               dest(dest), type(type) {}


void phiInstruction::emit(bool is_global) {
    string inst = dest + " = phi " + type  +" [" + val1 + ", " + label1 +"], ["+
                  val2 + ", " + label2 +"]";

    if (is_global) CodeBuffer::instance().emitGlobal(inst);
    else CodeBuffer::instance().emit(inst);
}


defineString::defineString(const string &src, const string& placeName){
    this->src = src.substr(1, src.length()-2);
    len = this->src.length()+1;
    this->placeName = placeName;
}
void defineString::emit(bool is_global){
    string inst = placeName+ " = constant [" + to_string(len)+" x i8] c\"" + src +
                  "\\00\"";
    CodeBuffer::instance().emitGlobal(inst);
}

unsigned long long defineString::getLen() {
    return len;
}
//
// int main(){
//
//    binopInstruction a("%a", "%b", "%c", "i32", "add");
//    a.emit();
//    binopInstruction b("%a", "%b", "%c", "i32", "sub");
//    b.emit();
//    binopInstruction m("%a", "%b", "%c", "i32", "mul");
//    m.emit();
//    binopInstruction c("%a", "%b", "%c", "i32", "sdiv");
//    c.emit();
//    binopInstruction d("%a", "%b", "%c", "i32", "sdiv");
//    d.emit();
//    allocateArrayInstruction p("ptr", "i8", 4);
//    p.emit();
//    getElementPtrInstruction g("%dest", "%ptr", "i8", 4, "0", "0" );
//    g.emit();
//    storeInstruction s("%element_ptr", "i32", "%init_index");
//    s.emit();
//
//    loadInstruction l("%print_ptr", "%element", "i32");
//    l.emit();
//    cmpInstruction tt("%size", "%print_index_inc","%print_loop_cond", "i32",
//            "slt" );
//    tt.emit();
//    cmpInstruction dd("%0", "1", "%1", "i32", "sle");
//    dd.emit();
//    unconditionalBrInstruction br("init_loop");
//    br.emit();
//    conditionalBrInstruction cbr("print_loop", "finish",
//            "%print_loop_cond");
//    cbr.emit();
//
//    vector<string> types1 = {"INT"};
//    vector<string> places1 = {"%2"};
//    callInstruction call1(types1, places1, "fn_fib", "int","%4");
//    call1.emit();
//    vector<string> types2 = {"STRING", "INT"};
//    vector<string> places2 = {"%msg", "12"};
//    callInstruction call2(types2, places2, "printf", "INT","%4");
//    call2.emit();
//     vector<string> types3 = {"INT"};
//     vector<string> places3 = {"%element"};
//     callInstruction call3(types3, places3, "printi", "VOID","");
//     call3.emit();
//     defineFuncInstruction def1(types1, "fn_fib", "INT");
//     def1.emit();
//     vector<string> typesDef2 = {"INT", "INT", "INT"};
//     defineFuncInstruction def2(typesDef2, "add_3_numbers", "i32");
//     def2.emit();
//     defineFuncInstruction def3(vector<string>(),"try", "VOID");
//     def3.emit();
//    CodeBuffer::instance().printCodeBuffer();
//}


