#include "Assembler.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "usage: Assembler Prog.asm" << endl;
        return 0;
    }
    
    std::string output_filename(argv[1]);
    if (output_filename.size() < 5 && output_filename.substr(output_filename.size()-5, 4) == ".asm"){
        cerr << "unknown filetype" << endl;
        return -1;
    }
    output_filename = output_filename.substr(0, output_filename.size()-4) + ".hack";
    try{
        Parser parser(argv[1]);
        std::ofstream ofs(output_filename, ios::out);
        Code code;
        std::string binary_buf;
        while (parser.hasMoreCommands()){
            parser.advance();
            if (parser.commandType() == A_COMMAND){
                ofs << "0" << code.dec2bin(parser.symbol()) << endl;;
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

