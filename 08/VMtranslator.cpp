#include "VMtranslator.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "usage: VMtranslator source" << std::endl;
        return 0;
    }

    try{
        std::string fileName(argv[1]);
        std::string filePath;
        std::vector<std::string> file_list;
        if (fileName.size()>3 && fileName.substr(fileName.size()-3, 3) == ".vm"){
            file_list.push_back(fileName);
            filePath = "";
        }else{
            DIR *dp;
            dirent* entry;
            dp = opendir(argv[1]);
            if (dp == NULL) exit(1);
            do {
                entry = readdir(dp);
                if (entry != NULL){
                    std::string tmp_name(entry->d_name);
                    if (tmp_name.size()>3 && tmp_name.substr(tmp_name.size()-3,3) == ".vm") {
                        file_list.push_back(tmp_name);
                    }
                }
            } while (entry != NULL);
            filePath = fileName;
        }

        CodeWriter code_writer(filePath+fileName);
        code_writer.writeInit();

        for (auto file_itr = file_list.begin(); file_itr!=file_list.end(); ++file_itr) {

            Parser parser(filePath+*file_itr);

            code_writer.setFileName(*file_itr);

            while (parser.hasMoreCommands()){
                parser.advance();
                if (parser.commandType() == C_PUSH || parser.commandType() == C_POP) {
                    code_writer.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
                }else if (parser.commandType() == C_LABEL){
                    code_writer.writeLabel(parser.arg1());
                }else if (parser.commandType() == C_GOTO){
                    code_writer.writeGoto(parser.arg1());
                }else if (parser.commandType() == C_IF){
                    code_writer.writeIf(parser.arg1());
                }else if (parser.commandType() == C_CALL){
                    code_writer.writeCall(parser.arg1(), parser.arg2());
                }else if (parser.commandType() == C_RETURN){
                    code_writer.writeReturn();
                }else if (parser.commandType() == C_FUNCTION){
                    code_writer.writeFunction(parser.arg1(), parser.arg2());
                }else if (parser.commandType() == C_ARITHMETIC){
                    code_writer.writeArithmetic(parser.arg1());
                }
            }
        }

        std::cout << "completed" << std::endl;
        return 0;
    }catch(std::exception& e){
        std::cerr << typeid(e).name() << "exceprtion throwed\n";
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

