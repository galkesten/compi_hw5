//
// Created by galkesten on 20/06/2021.
//

#ifndef COMPI_HW3_INSTRUCTIONS_H
#define COMPI_HW3_INSTRUCTIONS_H

#include <string>
#include <vector>
#include "bp.hpp"
using std::string;
using std::vector;
class Instruction {

public:
    Instruction()=default;
    ~Instruction() = default;
    virtual void emit(bool is_global= false) = 0;


};

class binopInstruction : Instruction{
    string src1;
    string src2;
    string dest;
    string type;
    string instruction;

public:
    binopInstruction(const string& src1, const string& src2, const string&
    dest, const string& type, const string& instruction);

    ~binopInstruction()= default;
    binopInstruction& operator=( const binopInstruction&) = default;
    binopInstruction(const binopInstruction&) = default;

    void emit(bool is_global= false) override;

};


class zextInstruction : Instruction{
    string dest;
    string src;
    string type_src;
    string type_dest;

public:
    zextInstruction(const string& dest, const string& src, const string type_src, const string& type_dest);
    ~zextInstruction() = default;
    zextInstruction& operator=(const zextInstruction&) = default;
    zextInstruction(const zextInstruction&) = default;
    void emit(bool is_global= false) override;

};


class truncInstruction : Instruction{
    string dest;
    string src;
    string type_src;
    string type_dest;

public:
    truncInstruction(const string& dest, const string& src, const string type_src, const string& type_dest);
    ~truncInstruction() = default;
    truncInstruction& operator=(const truncInstruction&) = default;
    truncInstruction(const truncInstruction&) = default;
    void emit(bool is_global= false) override;

};


class allocateVarInstruction : Instruction {

    string ptrName;
    string type;
public:

    allocateVarInstruction(const string& ptrName, const string& type);
    allocateVarInstruction(const allocateVarInstruction& ) = default;
    allocateVarInstruction& operator=(const allocateVarInstruction&) = default;
    ~allocateVarInstruction() = default;

    void emit(bool is_global= false) override;

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

    void emit(bool is_global= false) override;

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

    void emit(bool is_global= false) override;

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

    void emit(bool is_global= false) override;

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

    void emit(bool is_global= false) override;

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

    void emit(bool is_global= false) override;

};

class unconditionalBrInstruction : Instruction {

    string label;
public:

    unconditionalBrInstruction(const string& label);
    unconditionalBrInstruction(const unconditionalBrInstruction&) = default;
    unconditionalBrInstruction& operator=(const unconditionalBrInstruction&) = default;
    ~unconditionalBrInstruction() = default;

    void emit(bool is_global= false) override;

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

    void emit(bool is_global= false) override;

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

    void emit(bool is_global= false) override;

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

    void emit(bool is_global= false) override;

};



class phiInstruction : Instruction{
    string label1;
    string label2;
    string val1;
    string val2;
    string dest;
    string type;

public:
    phiInstruction(const string& label1, const string& label2, const string&
    val1, const string& val2, const string& dest, const string& type);
    phiInstruction (const phiInstruction&) = default;
    phiInstruction& operator=(const phiInstruction&) = default;
    ~ phiInstruction() = default;

    void emit(bool is_global= false) override;

};


class defineString : Instruction{

    string src;
    string placeName;
    unsigned long long len;

public:
    defineString(const string& src, const string& placeName);
    defineString (const defineString&) = default;
    defineString& operator=(const defineString&) = default;
    ~defineString() = default;

    void emit(bool is_global= false) override;

    unsigned long long getLen();

};


string addPercent(const string& s);


#endif //COMPI_HW3_INSTRUCTIONS_H
