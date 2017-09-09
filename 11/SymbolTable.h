#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

enum VarKind {
    STATIC, FIELD, ARG, VAR, NONE,
};

class TableValue {
    private:
        std::string type_;
        VarKind kind_;
        int number_;
    public:
        TableValue();
        TableValue(std::string type_a, VarKind kind_a, int number_a);

        std::string type();
        VarKind kind();
        int number();
};

class SymbolTable {
    private:
        std::map<std::string, TableValue> classTable;
        std::map<std::string, TableValue>  methodTable;
        int currentStaticNumber;
        int currentFieldNumber;
        int currentArgNumber;
        int currentVarNumber;
    public:
        SymbolTable();

        void startSubroutine();
        void define(std::string name, std::string type, VarKind kind);
        int varCount(VarKind kind);
        VarKind kindOf(std::string name);
        std::string typeOf(std::string name);
        int indexOf(std::string name);

        VarKind string2VarKind(std::string name_kind);
};


#endif /* SYMBOLTABLE_H */
