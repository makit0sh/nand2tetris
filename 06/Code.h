#ifndef CODE_H_
#define CODE_H_

#include <iostream>
#include <string>
#include <math.h>

class Code {
    public:
        std::string dest(std::string mnemonic);
        std::string comp(std::string mnemonic);
        std::string jump(std::string mnemonic);
        std::string dec2bin(std::string dec_symbol);
};

#endif // CODE_H_
