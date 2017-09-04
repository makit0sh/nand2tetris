#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
    stable["SP"] = 0;
    stable["LCL"] = 1;
    stable["ARG"] = 2;
    stable["THIS"] = 3;
    stable["THAT"] = 4;
    stable["R0"] = 0;
    stable["R1"] = 1;
    stable["R2"] = 2;
    stable["R3"] = 3;
    stable["R4"] = 4;
    stable["R5"] = 5;
    stable["R6"] = 6;
    stable["R7"] = 7;
    stable["R8"] = 8;
    stable["R9"] = 9;
    stable["R10"] = 10;
    stable["R11"] = 11;
    stable["R12"] = 12;
    stable["R13"] = 13;
    stable["R14"] = 14;
    stable["R15"] = 15;
    stable["SCREEN"] = 16384;
    stable["KBD"] = 24576;
}

void SymbolTable::addEntry(std::string symbol, int address) throw(std::runtime_error)
{
    if (contains(symbol)) {
        throw std::runtime_error("redundunt symbol found");
    }else{
        stable[symbol] = address;
    }
}

bool SymbolTable::contains(std::string symbol)
{
    return stable.find(symbol) != stable.end();
}

int SymbolTable::getAddress(std::string symbol) throw(std::runtime_error)
{
    if (contains(symbol)){
        return stable[symbol];
    }else{
        throw std::runtime_error("cannnot resolve symbol");
    }
}
