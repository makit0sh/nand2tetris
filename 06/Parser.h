#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <string>
#include <iterator>
#include <fstream>
#include <boost/algorithm/string.hpp>

enum CommandType
{
    A_COMMAND, C_COMMAND, L_COMMAND,
};

class Parser {
    private:
        std::ifstream ifs;
        CommandType command_type;
        std::string command;
    public:
        Parser(char* filename);
        ~Parser();

        bool hasMoreCommands();
        void advance();
        CommandType commandType();
        std::string symbol();
        std::string dest();
        std::string comp();
        std::string jump();
        void rewind();
};

#endif // PARSER_H_
