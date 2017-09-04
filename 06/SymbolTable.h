#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <iostream>
#include <string>
#include <map>

class SymbolTable {
    private:
        std::map<std::string, int> stable;
    public:
        SymbolTable();

        void addEntry(std::string symbol, int address) throw (std::runtime_error);
        bool contains(std::string symbol);
        int getAddress(std::string symbol) throw (std::runtime_error);
};

#endif // SYMBOLTABLE_H_
