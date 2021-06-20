//
// Created by galkesten on 20/06/2021.
//

#ifndef COMPI_HW3_INSTRUCTIONS_H
#define COMPI_HW3_INSTRUCTIONS_H

#include <string>
#include <vector>
using std::string;
using std::vector;
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
    dest, const string& type);

    ~addInstruction()= default;
    addInstruction& operator=( const addInstruction&) = default;
    addInstruction(const addInstruction&) = default;

    void emit() override;
};

class subInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;

public:
    subInstruction(const string& src1, const string& src2, const string&
    dest, const string& type);

    ~subInstruction()= default;
    subInstruction(const subInstruction&) = default;
    subInstruction& operator=(const subInstruction&)= default;

    void emit() override;
};


class mulInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;

public:
    mulInstruction(const string& src1, const string& src2, const string&
    dest, const string& type);

    ~mulInstruction()= default;
    mulInstruction(const mulInstruction&) = default;
    mulInstruction& operator=(const mulInstruction&) = default;
    void emit() override;
};


class unsignedDivInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;

public:
    unsignedDivInstruction(const string& src1, const string& src2,
                           const string& dest, const string& type);

    ~unsignedDivInstruction()= default;
    unsignedDivInstruction& operator=( const unsignedDivInstruction&) =default;
    unsignedDivInstruction(const unsignedDivInstruction&) = default;

    void emit() override;
};

class signedDivInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;

public:
    signedDivInstruction(const string& src1, const string& src2,
                         const string& dest, const string& type);
    ~signedDivInstruction()= default;
    signedDivInstruction(const signedDivInstruction&) = default;
    void emit() override;
};


class allocateVarInstruction : Instruction {

    string ptrName;
    string type;
public:

    allocateVarInstruction(const string& ptrName, const string& type);
    allocateVarInstruction(const allocateVarInstruction& ) = default;
    allocateVarInstruction& operator=(const allocateVarInstruction&) = default;
    ~allocateVarInstruction() = default;

    void emit() override;
};

class allocateArrayInstruction : Instruction {

    string ptrName; // ptr will be from type int 32*
    string type;
    long long size;

public:

    allocateArrayInstruction(const string& ptrName, const string& type, long
    long size);
    allocateArrayInstruction (const allocateArrayInstruction& ) = default;
    allocateArrayInstruction& operator=(const allocateArrayInstruction&) = default;
    ~allocateArrayInstruction() = default;

    void emit() override;
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
    targetArrayIndex, const string& targetElementIndex);
    getElementPtrInstruction (const getElementPtrInstruction& ) = default;
    getElementPtrInstruction& operator=(const getElementPtrInstruction&) = default;
    ~getElementPtrInstruction() = default;

    void emit() override;
};

class storeInstruction : Instruction {

    string ptrName;
    string type;
    string val;
public:

    storeInstruction(const string& ptrName, const string& type, const string&
    val);
    storeInstruction(const storeInstruction&) = default;
    storeInstruction& operator=(const storeInstruction&) = default;
    ~storeInstruction() = default;

    void emit() override;
};

class loadInstruction : Instruction {

    string srcPtr;
    string destPtr;
    string type;
public:

    loadInstruction(const string& srcPtr,const string&
    destPtr, const string& type);
    loadInstruction(const loadInstruction&) = default;
    loadInstruction& operator=(const loadInstruction&) = default;
    ~loadInstruction() = default;

    void emit() override;
};


class cmpInstruction : Instruction{

    string src1;
    string src2;
    string dest;
    string type;
    string cond;

public:
    cmpInstruction(const string& src1, const string& src2, const string&
    dest, const string& type, const string& cond);

    ~cmpInstruction()= default;
    cmpInstruction& operator=( const cmpInstruction&) = default;
    cmpInstruction(const cmpInstruction&) = default;

    void emit() override;
};

class unconditionalBrInstruction : Instruction {

    string label;
public:

    unconditionalBrInstruction(const string& label);
    unconditionalBrInstruction(const unconditionalBrInstruction&) = default;
    unconditionalBrInstruction& operator=(const unconditionalBrInstruction&) = default;
    ~unconditionalBrInstruction() = default;

    void emit() override;
};

class conditionalBrInstruction : Instruction {

    string trueLabel;
    string falseLabel;
    string regOfCmpRes;
public:

    conditionalBrInstruction(const string& trueLabel, const string&
    falseLabel, const string& regOfCmpRes);
    conditionalBrInstruction& operator=(const conditionalBrInstruction&) = default;
    ~conditionalBrInstruction() = default;

    void emit() override;
};

class callInstruction : Instruction {

    vector<string> places;
    vector<string> types;
    string funcName;
    string retType;
    string dest;

    string createParamsString();
public:

    callInstruction(const vector<string>& types, const vector<string>& places,
                    const string& funcName, const string& retType, const
                    string& dest="");
    callInstruction(const callInstruction&) = default;
    callInstruction& operator=(const callInstruction&) = default;
    ~callInstruction() = default;

    void emit() override;
};

class defineFuncInstruction : Instruction {

    vector<string> types;
    string funcName;
    string retType;

    string createParamsString();
public:

    defineFuncInstruction(const vector<string>& types,const string& funcName,
            const string& retType);
    defineFuncInstruction (const defineFuncInstruction&) = default;
    defineFuncInstruction& operator=(const defineFuncInstruction&) = default;
    ~defineFuncInstruction() = default;

    void emit() override;
};

string addPercent(const string& s);


#endif //COMPI_HW3_INSTRUCTIONS_H
