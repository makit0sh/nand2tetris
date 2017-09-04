#include "Assembler.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "usage: Assembler Prog.asm" << std::endl;
        return 0;
    }
    
    std::string output_filename(argv[1]);
    if (output_filename.size() < 5 && output_filename.substr(output_filename.size()-5, 4) == ".asm"){
        std::cerr << "unknown filetype" << std::endl;
        return -1;
    }
    output_filename = output_filename.substr(0, output_filename.size()-4) + ".hack";
    try{
        Parser parser(argv[1]);
        std::ofstream ofs(output_filename, std::ios::out);
        Code code;
        SymbolTable symbol_table;

        int line=-1;
        while (parser.hasMoreCommands()){
            parser.advance();
            if (parser.commandType() == A_COMMAND || parser.commandType() == C_COMMAND) {
                line++;
            }else if (parser.commandType() == L_COMMAND){
                symbol_table.addEntry(parser.symbol(), line+1);
            }
        }

        parser.rewind();
        int empty_address=16;;
        while (parser.hasMoreCommands()){
            parser.advance();
            if (parser.commandType() == A_COMMAND){
                try{
                    stoi(parser.symbol());
                    ofs << "0" << code.dec2bin(parser.symbol()) << std::endl;
                }catch(...){
                    if (!symbol_table.contains(parser.symbol())){
                        symbol_table.addEntry(parser.symbol(), empty_address);
                        empty_address++;
                    }
                    ofs << "0" << code.dec2bin( std::to_string( symbol_table.getAddress(parser.symbol()) ) ) << std::endl;
                }
            }else if (parser.commandType() == C_COMMAND){
                ofs << "111" << code.comp(parser.comp()) << code.dest(parser.dest()) << code.jump(parser.jump()) << std::endl;
            }
        }


        ofs.close();

        return 0;
    }catch(std::exception& e){
        std::cerr << typeid(e).name() << "exceprtion throwed\n";
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

