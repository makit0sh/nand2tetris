#include "JackAnalyzer.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "usage: JackAnalyzer source" << std::endl;
        return 0;
    }

    try{
        std::string sourceName(argv[1]);
        std::string filePath;
        std::vector<std::string> file_list;
        if (sourceName.size()>5 && sourceName.substr(sourceName.size()-5, 5) == ".jack"){
            file_list.push_back(sourceName);
            filePath = "";
        }else if (sourceName[sourceName.size()-1] == '/') {
            DIR *dp;
            dirent* entry;
            dp = opendir(argv[1]);
            if (dp == NULL) exit(1);
            do {
                entry = readdir(dp);
                if (entry != NULL){
                    std::string tmp_name(entry->d_name);
                    if (tmp_name.size()>5 && tmp_name.substr(tmp_name.size()-5,5) == ".jack") {
                        file_list.push_back(tmp_name);
                    }
                }
            } while (entry != NULL);
            filePath = sourceName;
        }else{
            throw std::runtime_error("source name should end with either \".jack\" or \"/\"");
        }


        for (auto file_itr = file_list.begin(); file_itr!=file_list.end(); ++file_itr) {

            JackTokenizer tokenizer(filePath+*file_itr);
            tokenizer.xml_output(filePath + file_itr->substr(0,file_itr->size()-5) +"T.xml");
            CompilationEngine cengine(tokenizer, filePath + file_itr->substr(0,file_itr->size()-5) +".xml");
            cengine.compileClass();
        }

        std::cout << "completed" << std::endl;
        return 0;
    }catch(std::exception& e){
        std::cerr << typeid(e).name() << "\n";
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

