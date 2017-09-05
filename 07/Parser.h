#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>

enum CommandType
{
    C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL, 
};

class Parser {
    private:
        std::ifstream ifs;
        CommandType command_type;
        std::string command;
        std::vector<std::string> command_list;
    public:
        Parser(char* filename);
        ~Parser();

        bool hasMoreCommands();
        void advance();
        CommandType commandType();
        std::string arg1();
        int arg2();
        void rewind();
};

#endif // PARSER_H_
