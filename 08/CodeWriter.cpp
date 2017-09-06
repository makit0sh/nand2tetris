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

    //TODO
    className = fileName.substr(0, fileName.size()-4);
    currentFunctionName = "";

    ofs = new std::ofstream(fileName);
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
        *ofs << "@$$JUMP" << labelcounter << std::endl;

        *ofs << "D;JEQ" << std::endl;

        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=0" << std::endl;
        *ofs << "@$$END" << labelcounter << std::endl;
        *ofs << "0;JMP" << std::endl;

        *ofs << "($$JUMP" << labelcounter << ")" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=-1" << std::endl;

        *ofs << "($$END" << labelcounter << ")" << std::endl;
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
        *ofs << "@$$JUMP" << labelcounter << std::endl;

        *ofs << "D;JLT" << std::endl;

        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=0" << std::endl;
        *ofs << "@$$END" << labelcounter << std::endl;
        *ofs << "0;JMP" << std::endl;

        *ofs << "($$JUMP" << labelcounter << ")" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=-1" << std::endl;

        *ofs << "($$END" << labelcounter << ")" << std::endl;
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
        *ofs << "@$$JUMP" << labelcounter << std::endl;

        *ofs << "D;JGT" << std::endl;

        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=0" << std::endl;
        *ofs << "@$$END" << labelcounter << std::endl;
        *ofs << "0;JMP" << std::endl;

        *ofs << "($$JUMP" << labelcounter << ")" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=-1" << std::endl;

        *ofs << "($$END" << labelcounter << ")" << std::endl;
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
            *ofs << "@" << index << std::endl;
            *ofs << "D=A" << std::endl;
        }else if (segment == "local") {
            *ofs << "@LCL" << std::endl;
            *ofs << "D=M" << std::endl;
            *ofs << "@" << index << std::endl;
            *ofs << "A=D+A" << std::endl;
            *ofs << "D=M" << std::endl;
        }else if (segment == "argument") {
            *ofs << "@ARG" << std::endl;
            *ofs << "D=M" << std::endl;
            *ofs << "@" << index << std::endl;
            *ofs << "A=D+A" << std::endl;
            *ofs << "D=M" << std::endl;

        }else if (segment == "this") {
            *ofs << "@THIS" << std::endl;
            *ofs << "D=M" << std::endl;
            *ofs << "@" << index << std::endl;
            *ofs << "A=D+A" << std::endl;
            *ofs << "D=M" << std::endl;

        }else if (segment == "that") {
            *ofs << "@THAT" << std::endl;
            *ofs << "D=M" << std::endl;
            *ofs << "@" << index << std::endl;
            *ofs << "A=D+A" << std::endl;
            *ofs << "D=M" << std::endl;

        }else if (segment == "pointer") {
            *ofs << "@3" << std::endl;
            *ofs << "D=A" << std::endl;
            *ofs << "@" << index << std::endl;
            *ofs << "A=D+A" << std::endl;
            *ofs << "D=M" << std::endl;

        }else if (segment == "temp") {
            *ofs << "@5" << std::endl;
            *ofs << "D=A" << std::endl;
            *ofs << "@" << index << std::endl;
            *ofs << "A=D+A" << std::endl;
            *ofs << "D=M" << std::endl;

        }else if (segment == "static") {
            *ofs << "@" << className << "." << index << std::endl;
            *ofs << "D=M" << std::endl;
        }else{
            throw std::runtime_error("invalid syntax");
        }
        *ofs << "@SP" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=D" << std::endl;
        *ofs << "D=A+1" << std::endl;
        *ofs << "@SP" << std::endl;
        *ofs << "M=D" << std::endl;

    }else if (command== C_POP){
        if (segment == "local") {
            *ofs << "@LCL" << std::endl;
            *ofs << "D=M" << std::endl;
        }else if (segment == "argument") {
            *ofs << "@ARG" << std::endl;
            *ofs << "D=M" << std::endl;
        }else if (segment == "this") {
            *ofs << "@THIS" << std::endl;
            *ofs << "D=M" << std::endl;
        }else if (segment == "that") {
            *ofs << "@THAT" << std::endl;
            *ofs << "D=M" << std::endl;
        }else if (segment == "pointer") {
            *ofs << "@3" << std::endl;
            *ofs << "D=A" << std::endl;
        }else if (segment == "temp") {
            *ofs << "@5" << std::endl;
            *ofs << "D=A" << std::endl;
        }else if (segment == "static"){
            *ofs << "@" << className << "." << index << std::endl;
            *ofs << "D=A" << std::endl;
        }else{
            throw std::runtime_error("invalid syntax");
        }
        if (segment != "static") {
            *ofs << "@" << index << std::endl;
            *ofs << "D=D+A" << std::endl;
        }
        *ofs << "@R13" << std::endl;
        *ofs << "M=D" << std::endl;

        *ofs << "@SP" << std::endl;
        *ofs << "AM=M-1" << std::endl;
        *ofs << "D=M" << std::endl;
        *ofs << "@R13" << std::endl;
        *ofs << "A=M" << std::endl;
        *ofs << "M=D" << std::endl;

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

void CodeWriter::writeInit()
{
    *ofs << "@256" << std::endl;
    *ofs << "D=A" << std::endl;
    *ofs << "@SP" << std::endl;
    *ofs << "M=D" << std::endl;
    writeCall("Sys.init", 0);
}

void CodeWriter::writeLabel(std::string label)
{
    *ofs << "(" << currentFunctionName << "$" << label << ")" << std::endl;
}

void CodeWriter::writeGoto(std::string label)
{
    *ofs << "@" << currentFunctionName << "$" << label << std::endl;
    *ofs << "0;JMP" << std::endl;
}

void CodeWriter::writeIf(std::string label)
{
    writePushPop(C_POP, "temp", 0);
    *ofs << "@5" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@" << currentFunctionName << "$" << label << std::endl;
    *ofs << "D;JNE" << std::endl;
}

void CodeWriter::writeCall(std::string functionName, int numArgs)
{
    std::string return_address_label = "$return-address" + std::to_string(labelcounter);
    labelcounter++;
    *ofs << "@" << return_address_label << std::endl;
    *ofs << "D=A" << std::endl;
    *ofs << "@5" << std::endl;
    *ofs << "M=D" << std::endl;
    writePushPop(C_PUSH, "temp", 0);
    *ofs << "@LCL" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@5" << std::endl;
    *ofs << "M=D" << std::endl;
    writePushPop(C_PUSH, "temp", 0);
    *ofs << "@ARG" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@5" << std::endl;
    *ofs << "M=D" << std::endl;
    writePushPop(C_PUSH, "temp", 0);
    *ofs << "@THIS" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@5" << std::endl;
    *ofs << "M=D" << std::endl;
    writePushPop(C_PUSH, "temp", 0);
    *ofs << "@THAT" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@5" << std::endl;
    *ofs << "M=D" << std::endl;
    writePushPop(C_PUSH, "temp", 0);
    *ofs << "@SP" << std::endl;
    *ofs << "D=A" << std::endl;
    *ofs << "@5" << std::endl;
    *ofs << "D=D-A" << std::endl;
    *ofs << "@" << numArgs << std::endl;
    *ofs << "D=D-A" << std::endl;
    *ofs << "@ARG" << std::endl;
    *ofs << "M=D" << std::endl;
    *ofs << "@SP" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@LCL" << std::endl;
    *ofs << "M=D" << std::endl;
    *ofs << "@" << functionName << std::endl;
    *ofs << "0;JMP" << std::endl;
    *ofs << "(" << return_address_label << ")" << std::endl;
}

void CodeWriter::writeReturn()
{
    currentFunctionName = "";

    *ofs << "@LCL" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@R14" << std::endl; //FRAME
    *ofs << "M=D" << std::endl;
    *ofs << "@5" << std::endl;
    *ofs << "A=D-A" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@R15" << std::endl; //RET
    *ofs << "M=D" << std::endl;
    writePushPop(C_POP, "argument", 0);
    *ofs << "@ARG" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@SP" << std::endl;
    *ofs << "M=D+1" << std::endl;
    *ofs << "@R14" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@1" << std::endl;
    *ofs << "A=D-A" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@THAT" << std::endl;
    *ofs << "M=D" << std::endl;
    *ofs << "@R14" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@2" << std::endl;
    *ofs << "A=D-A" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@THIS" << std::endl;
    *ofs << "M=D" << std::endl;
    *ofs << "@R14" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@3" << std::endl;
    *ofs << "A=D-A" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@ARG" << std::endl;
    *ofs << "M=D" << std::endl;
    *ofs << "@R14" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@4" << std::endl;
    *ofs << "A=D-A" << std::endl;
    *ofs << "D=M" << std::endl;
    *ofs << "@LCL" << std::endl;
    *ofs << "M=D" << std::endl;
    *ofs << "@R15" << std::endl;
    *ofs << "A=M" << std::endl;
    *ofs << "0;JMP" << std::endl;
}

void CodeWriter::writeFunction(std::string functionName, int numLocals)
{
    currentFunctionName = functionName;
    *ofs << "(" << functionName << ")" << std::endl;
    for (int i=0;i<numLocals;i++) {
        writePushPop(C_PUSH, "constant", 0);
    }
}


