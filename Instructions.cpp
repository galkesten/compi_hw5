//
// Created by galkesten on 19/06/2021.
//
#include "bp.hpp"
#include <string>
#include <vector>
#include <stdio.h>

string addPercent(const string& s){
    if (s[0]!='%'){
        return "%"+s;
    }
    return s;
}

class Instruction {

    public:
    Instruction()=default;
    ~Instruction() = default;
    virtual void emit() = 0;


};

class addInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;

    public:
    addInstruction(const string& src1, const string& src2, const string&
    dest, const string& type) :src1(src1),src2(src2), dest(dest), type(type){}

    ~addInstruction()= default;
    addInstruction& operator=( const addInstruction&) = default;
    addInstruction(const addInstruction&) = default;

     void emit() override {
        string inst = dest + " = add " + type+ " " + src1+", " + src2;
         CodeBuffer::instance().emit(inst);
    }
};

class subInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;

public:
    subInstruction(const string& src1, const string& src2, const string&
    dest, const string& type) : src1(src1),
    src2(src2), dest(dest), type(type){}

    ~subInstruction()= default;
    subInstruction(const subInstruction&) = default;
    subInstruction& operator=(const subInstruction&)= default;

    void emit() override {
        string inst = dest + " = sub " + type+ " " + src1+", " + src2;
        CodeBuffer::instance().emit(inst);
    }
};


class mulInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;

public:
    mulInstruction(const string& src1, const string& src2, const string&
    dest, const string& type) : src1(src1), src2(src2), dest(dest), type(type){}

    ~mulInstruction()= default;
    mulInstruction(const mulInstruction&) = default;
    mulInstruction& operator=(const mulInstruction&) = default;
    void emit() override {
        string inst = dest + " = mul " + type+ " " + src1+", " + src2;
        CodeBuffer::instance().emit(inst);
    }
};


class unsignedDivInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;

public:
    unsignedDivInstruction(const string& src1, const string& src2,
            const string& dest, const string& type) : src1(src1), src2(src2),
            dest(dest), type(type){}

    ~unsignedDivInstruction()= default;
    unsignedDivInstruction& operator=( const unsignedDivInstruction&) =default;
    unsignedDivInstruction(const unsignedDivInstruction&) = default;

    void emit() override {
        string inst = dest + " = udiv " + type+ " " + src1+", " + src2;
        CodeBuffer::instance().emit(inst);
    }
};

class signedDivInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;

public:
    signedDivInstruction(const string& src1, const string& src2,
            const string& dest, const string& type) : src1(src1), src2(src2),
            dest(dest), type
            (type){}

    ~signedDivInstruction()= default;
    signedDivInstruction(const signedDivInstruction&) = default;
    void emit() override {
        string inst = dest + " = sdiv " + type+ " " + src1+", " + src2;
        CodeBuffer::instance().emit(inst);
    }
};


class allocateVarInstruction : Instruction {

    string ptrName;
    string type;
    public:

    allocateVarInstruction(const string& ptrName, const string& type) : ptrName
    (ptrName){}
    allocateVarInstruction(const allocateVarInstruction& ) = default;
    allocateVarInstruction& operator=(const allocateVarInstruction&) = default;
    ~allocateVarInstruction() = default;

    void emit() override {
        string inst = ptrName + " = alloca "+type;
        CodeBuffer::instance().emit(inst);
    }
};

class allocateArrayInstruction : Instruction {

    string ptrName; // ptr will be from type int 32*
    string type;
    long long size;

public:

    allocateArrayInstruction(const string& ptrName, const string& type, long
    long size) : ptrName(ptrName), type(type), size(size){}
    allocateArrayInstruction (const allocateArrayInstruction& ) = default;
    allocateArrayInstruction& operator=(const allocateArrayInstruction&) = default;
    ~allocateArrayInstruction() = default;

    void emit() override {
        string inst = ptrName + " = alloca "+ type+ ", "+ type + " "+
        to_string(size);
        CodeBuffer::instance().emit(inst);
    }
};

class getElementPtrInstruction : Instruction {

    string destPtr; // ptr will be from type int 32*
    string srcPtr;
    string elemType;
    long long arraySize;
    string targetArrayIndex;
    string targetElementIndex;

public:

    getElementPtrInstruction(const string& destPtr, const string& srcPtr,
            const string& elemType, long long arraySize, const string&
            targetArrayIndex, const string& targetElementIndex) : Instruction(), destPtr(destPtr), srcPtr(srcPtr),
    elemType(elemType), arraySize(arraySize), targetArrayIndex
    (targetArrayIndex), targetElementIndex(targetElementIndex){}

    getElementPtrInstruction (const getElementPtrInstruction& ) = default;
    getElementPtrInstruction& operator=(const getElementPtrInstruction&) = default;
    ~getElementPtrInstruction() = default;

    void emit() override {
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
        CodeBuffer::instance().emit(inst);
    }
};

class storeInstruction : Instruction {

    string ptrName;
    string type;
    string val;
public:

    storeInstruction(const string& ptrName, const string& type, const string&
    val) : ptrName(ptrName), type(type), val(val){}
    storeInstruction(const storeInstruction&) = default;
    storeInstruction& operator=(const storeInstruction&) = default;
    ~storeInstruction() = default;

    void emit() override {
        string inst = "store " + type + " " + val+", "+ type + "*" + " "+ ptrName;
        CodeBuffer::instance().emit(inst);
    }
};

class loadInstruction : Instruction {

    string srcPtr;
    string destPtr;
    string type;
public:

    loadInstruction(const string& srcPtr,const string&
    destPtr, const string& type) : srcPtr(srcPtr), destPtr(destPtr), type
    (type){}
    loadInstruction(const loadInstruction&) = default;
    loadInstruction& operator=(const loadInstruction&) = default;
    ~loadInstruction() = default;

    void emit() override {
        string inst = destPtr + " = load " + type + ", "+ type +"* "+ srcPtr;
        CodeBuffer::instance().emit(inst);
    }
};


class cmpInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;
    string cond;

public:
    cmpInstruction(const string& src1, const string& src2, const string&
    dest, const string& type, const string& cond) :src1(src1),src2(src2), dest
    (dest), type(type), cond(cond){}

    ~cmpInstruction()= default;
     cmpInstruction& operator=( const cmpInstruction&) = default;
     cmpInstruction(const cmpInstruction&) = default;

    void emit() override {
        string inst = dest + " = icmp " + cond + " "+ type+ " " + src1+", " +
                src2;
        CodeBuffer::instance().emit(inst);
    }
};

class unconditionalBrInstruction : Instruction {

    string label;
public:

    unconditionalBrInstruction(const string& label) : label(addPercent(label)){}
    unconditionalBrInstruction(const unconditionalBrInstruction&) = default;
    unconditionalBrInstruction& operator=(const unconditionalBrInstruction&) = default;
    ~unconditionalBrInstruction() = default;

    void emit() override {
        string inst = "br label "+label;
        CodeBuffer::instance().emit(inst);
    }
};

class conditionalBrInstruction : Instruction {

    string trueLabel;
    string falseLabel;
    string regOfCmpRes;
public:

    conditionalBrInstruction(const string& trueLabel, const string&
    falseLabel, const string& regOfCmpRes) : trueLabel(addPercent(trueLabel)),
    falseLabel(addPercent(falseLabel)), regOfCmpRes(regOfCmpRes){}
    conditionalBrInstruction(const conditionalBrInstruction&) = default;
    conditionalBrInstruction& operator=(const conditionalBrInstruction&) = default;
    ~conditionalBrInstruction() = default;

    void emit() override {
        string inst = "br i1 " + regOfCmpRes+", label "+trueLabel+", label "
                                                                 ""+falseLabel;
        CodeBuffer::instance().emit(inst);
    }
};

class callInstruction : Instruction {

    vector<string> places;
    vector<string> types;
    string funcName;
    string retType;
    string dest;

    string createParamsString(){
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
public:

    callInstruction(const vector<string>& types, const vector<string>& places,
                    const string& funcName, const string& retType, const string& dest="") {
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
    callInstruction(const callInstruction&) = default;
    callInstruction& operator=(const callInstruction&) = default;
    ~callInstruction() = default;

    void emit() override {
        string inst;
        if(dest != ""){
            inst+= dest+ " = ";
        }
        string params = createParamsString();
        inst+= "call "+ retType +" "+ funcName+"("+params+")";
        CodeBuffer::instance().emit(inst);
    }
};

class defineFuncInstruction : Instruction {

    vector<string> types;
    string funcName;
    string retType;

    string createParamsString(){
        string params="";
        for(int i=0; i< types.size(); ++i){
            params+= types[i];
            if(i!=types.size()-1){
                params+=", ";
            }
        }
        return params;
    }
public:

    defineFuncInstruction(const vector<string>& types,const string& funcName, const string& retType) {
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
    defineFuncInstruction (const defineFuncInstruction&) = default;
    defineFuncInstruction& operator=(const defineFuncInstruction&) = default;
    ~defineFuncInstruction() = default;

    void emit() override {
        string params = createParamsString();
        string inst = "define "+ retType+ " "+funcName +"("+params+")"+" {";
        CodeBuffer::instance().emit(inst);
    }
};

 int main(){

    addInstruction a("%a", "%b", "%c", "i32");
    a.emit();
    subInstruction b("%a", "%b", "%c", "i32");
    b.emit();
    mulInstruction m("%a", "%b", "%c", "i32");
    m.emit();
    signedDivInstruction c("%a", "%b", "%c", "i32");
    c.emit();
    unsignedDivInstruction d("%a", "%b", "%c", "i32");
    d.emit();
    allocateArrayInstruction p("ptr", "i8", 4);
    p.emit();
    getElementPtrInstruction g("%dest", "%ptr", "i8", 4, "0", "0" );
    g.emit();
    storeInstruction s("%element_ptr", "i32", "%init_index");
    s.emit();

    loadInstruction l("%print_ptr", "%element", "i32");
    l.emit();
    cmpInstruction tt("%size", "%print_index_inc","%print_loop_cond", "i32",
            "slt" );
    tt.emit();
    cmpInstruction dd("%0", "1", "%1", "i32", "sle");
    dd.emit();
    unconditionalBrInstruction br("init_loop");
    br.emit();
    conditionalBrInstruction cbr("print_loop", "finish",
            "%print_loop_cond");
    cbr.emit();

    vector<string> types1 = {"INT"};
    vector<string> places1 = {"%2"};
    callInstruction call1(types1, places1, "fn_fib", "int","%4");
    call1.emit();
    vector<string> types2 = {"STRING", "INT"};
    vector<string> places2 = {"%msg", "12"};
    callInstruction call2(types2, places2, "printf", "INT","%4");
    call2.emit();
     vector<string> types3 = {"INT"};
     vector<string> places3 = {"%element"};
     callInstruction call3(types3, places3, "printi", "VOID","");
     call3.emit();
     defineFuncInstruction def1(types1, "fn_fib", "INT");
     def1.emit();
     vector<string> typesDef2 = {"INT", "INT", "INT"};
     defineFuncInstruction def2(typesDef2, "add_3_numbers", "i32");
     def2.emit();
     defineFuncInstruction def3(vector<string>(),"try", "VOID");
     def3.emit();
    CodeBuffer::instance().printCodeBuffer();
}


