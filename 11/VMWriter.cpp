#include "VMWriter.h"

VMWriter::VMWriter(std::string fileName) : ofs(fileName)
{
    ofs.exceptions(std::ifstream::failbit);
    std::cout << "compiling to file: "<< fileName << std::endl;
}

void VMWriter::writePush(std::string segment, int index)
{
    ofs << "push " << segment << " " << index << std::endl;
}

void VMWriter::writePop(std::string segment, int index)
{
    ofs << "pop " << segment << " " << index << std::endl;
}

void VMWriter::writeArithmetic(std::string command)
{
    ofs << command << std::endl;
}

void VMWriter::writeLabel(std::string label)
{
    ofs << "label " << label << std::endl;
}

void VMWriter::writeGoto(std::string label)
{
    ofs << "goto " << label << std::endl;
}

void VMWriter::writeIf(std::string label)
{
    ofs << "if-goto " << label << std::endl;
}

void VMWriter::writeCall(std::string name, int nArgs)
{
    ofs << "call " << name << " " << nArgs << std::endl;
}

void VMWriter::writeFunction(std::string name, int nLocals)
{
    ofs << "function " << name << " " << nLocals << std::endl;
}

void VMWriter::writeReturn()
{
    ofs << "return" << std::endl;
}

void VMWriter::close()
{
    ofs.close();
}


