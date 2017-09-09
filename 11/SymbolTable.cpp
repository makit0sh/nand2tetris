#include "SymbolTable.h"

TableValue::TableValue(){}

TableValue::TableValue(std::string type_a, VarKind kind_a, int number_a)
{
    type_ = type_a;
    kind_ = kind_a;
    number_ = number_a;
}

std::string TableValue::type()
{
    return type_;
}

VarKind TableValue::kind()
{
    return kind_;
}

int TableValue::number()
{
    return number_;
}

SymbolTable::SymbolTable()
    : currentStaticNumber(0), currentFieldNumber(0), currentArgNumber(0), currentVarNumber(0)
{
}

void SymbolTable::startSubroutine()
{
    methodTable.clear();
    currentArgNumber=0;
    currentVarNumber=0;
}

void SymbolTable::define(std::string name, std::string type, VarKind kind)
{
    if (classTable.find(name)!=classTable.end() || methodTable.find(name)!=methodTable.end()) throw std::runtime_error(name+" already defined");
    if (kind==STATIC) {
        TableValue value(type, kind, currentStaticNumber);
        classTable[name] = value;
        currentStaticNumber++;
    }else if (kind == FIELD) {
        TableValue value(type, kind, currentFieldNumber);
        classTable[name] = value;
        currentFieldNumber++;
    }else if (kind == ARG) {
        TableValue value(type, kind, currentArgNumber);
        methodTable[name] = value;
        currentArgNumber++;
    }else if (kind==VAR) {
        TableValue value(type, kind, currentVarNumber);
        methodTable[name] = value;
        currentVarNumber++;
    }else{
        throw std::runtime_error("unknown VarKind");
    }
}

int SymbolTable::varCount(VarKind kind)
{
    if (kind==STATIC) { return currentStaticNumber; }
    else if (kind==FIELD) { return currentFieldNumber; }
    else if (kind==ARG) { return currentArgNumber; }
    else if (kind==VAR) { return currentVarNumber; }
    else { throw std::runtime_error("unknown VarKind");}
}

VarKind SymbolTable::kindOf(std::string name)
{
    if (classTable.find(name)!=classTable.end()) {
        return classTable[name].kind();
    }else if (methodTable.find(name)!=methodTable.end()) {
        return methodTable[name].kind();
    }else{
        return NONE;
    }
}

std::string SymbolTable::typeOf(std::string name)
{
    if (classTable.find(name)!=classTable.end()) {
        return classTable[name].type();
    }else if (methodTable.find(name)!=methodTable.end()) {
        return methodTable[name].type();
    }else{
        throw std::runtime_error(name+" not defined");
    }
}

int SymbolTable::indexOf(std::string name)
{
    if (classTable.find(name)!=classTable.end()) {
        return classTable[name].number();
    }else if (methodTable.find(name)!=methodTable.end()) {
        return methodTable[name].number();
    }else{
        throw std::runtime_error(name+" not defined");
    }
}

VarKind SymbolTable::string2VarKind(std::string name_kind)
{
    if (name_kind=="static") { return STATIC; }
    else if (name_kind=="field") { return FIELD; }
    else if (name_kind=="arg") { return ARG; }
    else { return NONE; }
}
