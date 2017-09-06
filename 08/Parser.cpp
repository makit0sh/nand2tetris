#include "Parser.h"

Parser::Parser(char* filename) : ifs(filename)
{
    ifs.exceptions(std::ifstream::failbit);
}

Parser::~Parser()
{
    ifs.close();
}

bool Parser::hasMoreCommands()
{
    std::string tmp;
    int original_pos = ifs.tellg();
    while (std::getline(ifs, tmp)) {
        boost::trim(tmp);
        if (!(tmp.size()>=2 && (tmp.substr(0,2)=="//"))) {
            ifs.seekg(original_pos, std::ios_base::beg);
            return true;
        }
    }
    return false;
}

void Parser::advance()
{
    if (!hasMoreCommands()) {
        return;
    }

    std::getline(ifs, command);
    if (command.find("//") != std::string::npos ) {
        command = command.substr(0, command.find("//"));
    }
    boost::trim(command);
    boost::split(command_list, command, boost::is_space());

    if (command.empty()) {
        advance();
    }else if (command_list[0] == "push") {
        command_type = C_PUSH;
    }else if (command_list[0] == "pop") {
        command_type = C_POP;
    }else if (command_list[0] == "label") {
        command_type = C_LABEL;
    }else if (command_list[0] == "goto") {
        command_type = C_GOTO;
    }else if (command_list[0] == "if-goto") {
        command_type = C_IF;
    }else if (command_list[0] == "function") {
        command_type = C_FUNCTION;
    }else if (command_list[0] == "call") {
        command_type = C_CALL;
    }else if (command_list[0] == "return") {
        command_type = C_RETURN;
    }else{
        command_type = C_ARITHMETIC;
    }
}

CommandType Parser::commandType()
{
    return command_type;
}

std::string Parser::arg1()
{
    if (command_type == C_ARITHMETIC) {
        return command_list[0];
    }else if (command_type == C_RETURN) {
        throw std::runtime_error("arg1 not found");
    }else{
        return command_list[1];
    }
}

int Parser::arg2()
{
    if (command_type == C_PUSH || command_type == C_POP || command_type == C_FUNCTION || command_type == C_CALL) {
        return stoi(command_list[2]);
    }else{
        throw std::runtime_error("arg2 not found");
    }
}

void Parser::rewind()
{
    ifs.clear();
    ifs.seekg(0, std::ios_base::beg);
}
