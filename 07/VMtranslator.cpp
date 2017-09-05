#include "VMtranslator.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "usage: VMtranslator source" << std::endl;
        return 0;
    }
    
    std::string output_filename(argv[1]);
    if (output_filename.size() < 4 && output_filename.substr(output_filename.size()-4, 3) == ".vm"){
        std::cerr << "unknown filetype" << std::endl;
        return -1;
    }
    output_filename = output_filename.substr(0, output_filename.size()-3) + ".asm";
    
    try{
        Parser parser(argv[1]);
        CodeWriter code_writer;

        code_writer.setFileName(output_filename);

        while (parser.hasMoreCommands()){
            parser.advance();
            if (parser.commandType() == C_PUSH || parser.commandType() == C_POP) {
                code_writer.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
            }else if (parser.commandType() == C_ARITHMETIC){
                code_writer.writeArithmetic(parser.arg1());
            }
        }


        return 0;
    }catch(std::exception& e){
        std::cerr << typeid(e).name() << "exceprtion throwed\n";
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

