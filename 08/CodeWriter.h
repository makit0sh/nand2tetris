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
        std::string currentFunctionName;
    public:
        CodeWriter();
        ~CodeWriter();

        void setFileName(std::string fileName);
        void writeArithmetic(std::string command);
        void writePushPop(CommandType command, std::string segment, int index);
        void close();
        void writeInit();
        void writeLabel(std::string label);
        void writeGoto(std::string label);
        void writeIf(std::string label);
        void writeCall(std::string functionName, int numArgs);
        void writeReturn();
        void writeFunction(std::string functionName, int numLocals);
};

#endif // CODEWRITER_H_
