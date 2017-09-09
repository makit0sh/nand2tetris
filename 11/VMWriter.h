#ifndef VMWRITER_H
#define VMWRITER_H

#include <iostream>
#include <fstream>
#include <string>

/*
enum SegmentName {
    CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP
};

enum CommandName {
    ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT
};
*/

class VMWriter {
    private:
        std::ofstream ofs;
    public:
        VMWriter(std::string fileName);

        void writePush(std::string segment, int index);
        void writePop(std::string  segment, int index);
        void writeArithmetic(std::string command);
        void writeLabel(std::string label);
        void writeGoto(std::string label);
        void writeIf(std::string label);
        void writeCall(std::string name, int nArgs);
        void writeFunction(std::string name, int nLocals);
        void writeReturn();
        void close();
};


#endif /* VMWRITER_H */
