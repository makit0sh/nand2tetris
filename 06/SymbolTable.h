#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <iostream>
#include <string>

class SymbolTable {
    public:
        SymbolTable();

        void addEntry(std::string symbol, int address);
        bool contains(std::string symbol);
        int getAddress(std::string symbol);
};

#endif // SYMBOLTABLE_H_
