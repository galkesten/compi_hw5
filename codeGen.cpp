#include "codeGen.hpp"


codeGen &codeGen::instance(){
    static  codeGen inst;
    return inst;
}

string codeGen::newVar(){
    string var_name = "%var" + to_string(count_var);
    count_var++;
    return var_name;
}

// code_gen::newString(){
//    new_str = "@.str" + to_string(var_counter);
//    var_counter++;
//    return new_str;
//}

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

//void genString(const semanticAttributes& attribute) {
//    attribute.place = codeGen::instance().newVar();
//    binopInstruction inst(attribute.byteVal, to_string(0), attribute.place, "i32",
//                       "add");
//    inst.emit();
//}
//void genVar(const semanticAttribute& attribute){
//    attribute.place = code_gen::instance().newVar();
//    if (attribute.type=="BYTE") val=attribute.byteVar
//    if (attribute.type=="INT") val=attribute.intVar
//    if (attribute.type=="STRING") val=attribute.stringVar
//    binopInstruction b(val, toString(0),attribute.place, "i32", "add")
//}