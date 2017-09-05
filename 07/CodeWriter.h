#ifndef CODEWRITER_H_
#define CODEWRITER_H_

#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"

class CodeWriter {
    private:
        std::ofstream* ofs;
        int labelcounter;
        std::string className;
    public:
        CodeWriter();
        ~CodeWriter();

        void setFileName(std::string fileName);
        void writeArithmetic(std::string command);
        void writePushPop(CommandType command, std::string segment, int index);
        void close();
};

#endif // CODEWRITER_H_
