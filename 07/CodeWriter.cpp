#include "CodeWriter.h"

CodeWriter::CodeWriter() : ofs(nullptr)
{
}

CodeWriter::~CodeWriter()
{
    close();
}

void CodeWriter::setFileName(std::string fileName)
{
    if (ofs != nullptr) {
        close();
    }

    labelcounter = 0;
    ofs = new std::ofstream(fileName, std::ios::out);
    ofs->exceptions(std::ifstream::failbit);
}

void CodeWriter::writeArithmetic(std::string command)
{
    if (command == "add") {
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "A=A-1" << std::endl;
        *ofs << "D=M" << std::endl;
        *ofs << "A=A-1" << std::endl;
        *ofs << "M=D+M" << std::endl;
        *ofs << "D=A+1" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=D" << std::endl;
    }else if (command == "sub") {
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "A=A-1" << std::endl;
        *ofs << "D=M" << std::endl;
        *ofs << "A=A-1" << std::endl;
        *ofs << "M=D-M" << std::endl;
        *ofs << "M=-M" << std::endl;
        *ofs << "D=A+1" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=D" << std::endl;
    }else if (command == "neg") {
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "A=A-1" << std::endl;
        *ofs << "M=-M" << std::endl;
        *ofs << "D=A+1" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=D" << std::endl;
    }else if (command == "eq") {
        *ofs << "@SP" << std::endl;
        *ofs << "M=M-1" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "D=M" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=M-1" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "D=D-M" << std::endl;
        *ofs << "@JUMP" << labelcounter << std::endl;

        *ofs << "D;JEQ" << std::endl;

        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=0" << std::endl;
        *ofs << "@END" << labelcounter << std::endl;
        *ofs << "0;JMP" << std::endl;

        *ofs << "(JUMP" << labelcounter << ")" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=-1" << std::endl;

        *ofs << "(END" << labelcounter << ")" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=M+1" << std::endl;
        labelcounter++;

    }else if (command == "gt") {
        *ofs << "@SP" << std::endl;
        *ofs << "M=M-1" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "D=M" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=M-1" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "D=D-M" << std::endl;
        *ofs << "@JUMP" << labelcounter << std::endl;

        *ofs << "D;JLT" << std::endl;

        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=0" << std::endl;
        *ofs << "@END" << labelcounter << std::endl;
        *ofs << "0;JMP" << std::endl;

        *ofs << "(JUMP" << labelcounter << ")" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=-1" << std::endl;

        *ofs << "(END" << labelcounter << ")" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=M+1" << std::endl;
        labelcounter++;


    }else if (command == "lt") {
        *ofs << "@SP" << std::endl;
        *ofs << "M=M-1" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "D=M" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=M-1" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "D=D-M" << std::endl;
        *ofs << "@JUMP" << labelcounter << std::endl;

        *ofs << "D;JGT" << std::endl;

        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=0" << std::endl;
        *ofs << "@END" << labelcounter << std::endl;
        *ofs << "0;JMP" << std::endl;

        *ofs << "(JUMP" << labelcounter << ")" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=-1" << std::endl;

        *ofs << "(END" << labelcounter << ")" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=M+1" << std::endl;
        labelcounter++;


    }else if (command == "and") {
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "A=A-1" << std::endl;
        *ofs << "D=M" << std::endl;
        *ofs << "A=A-1" << std::endl;
        *ofs << "M=D&M" << std::endl;
        *ofs << "D=A+1" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=D" << std::endl;
    }else if (command == "or") {
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "A=A-1" << std::endl;
        *ofs << "D=M" << std::endl;
        *ofs << "A=A-1" << std::endl;
        *ofs << "M=D|M" << std::endl;
        *ofs << "D=A+1" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=D" << std::endl;
    }else if (command == "not") {
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "A=A-1" << std::endl;
        *ofs << "M=!M" << std::endl;
        *ofs << "D=A+1" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=D" << std::endl;
    }else{
        throw std::runtime_error("invalid syntax");
    }
}

void CodeWriter::writePushPop(CommandType command, std::string segment, int index)
{
    if (command== C_PUSH) {
        if (segment == "constant") {
            *ofs << "@" << std::to_string(index) << std::endl;
            *ofs << "D=A" << std::endl;
            *ofs << "@SP" << std::endl;
            *ofs << "A=M" << std::endl;
            *ofs << "M=D" << std::endl;
            *ofs << "D=A+1" << std::endl;
            *ofs << "@SP" << std::endl;
            *ofs << "M=D" << std::endl;
        }else{
            throw std::runtime_error("invalid syntax");
        }
    }else if (command== C_POP){
        if (segment == "constant") {
            *ofs << "@" << std::to_string(index) << std::endl;
            *ofs << "D=A" << std::endl;
            *ofs << "@SP" << std::endl;
            *ofs << "A=M" << std::endl;
            *ofs << "M=D" << std::endl;
            *ofs << "D=A+1" << std::endl;
            *ofs << "@SP" << std::endl;
            *ofs << "M=D" << std::endl;
        }else{
            throw std::runtime_error("invalid syntax");
        }
    }else{
        throw std::runtime_error("command was not push nor pop");
    }
}

void CodeWriter::close()
{
    if (ofs != nullptr) {
        ofs->close();
        ofs = nullptr;
    }
}


