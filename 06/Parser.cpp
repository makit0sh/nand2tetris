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

    if (command.empty()) {
        advance();
    }else if (command[0] == '@') {
        command_type = A_COMMAND;
    }else if (command.front() == '(' && command.back() == ')') {
        command_type = L_COMMAND;
    }else{
        command_type = C_COMMAND;
    }
}

CommandType Parser::commandType()
{
    return command_type;
}

std::string Parser::symbol()
{
    if (command_type == A_COMMAND){
        return command.substr(1, command.size()-1);
    }else if (command_type == L_COMMAND){
        return command.substr(1, command.size()-2);
    }else{
        return "";
    }
}

std::string Parser::dest()
{
    if (command_type == C_COMMAND){
        if (command.substr(0,2) == "A=" || command.substr(0,2) == "M=" || command.substr(0,2) == "D="){
            return command.substr(0,1);
        }else if (command.substr(0,3) == "MD=" || command.substr(0,3) == "AM=" ||command.substr(0,3) == "AD="){
            return command.substr(0,2);
        }else if (command.substr(0,4) == "AMD=") {
            return command.substr(0,3);
        }else{
            return "null";
        }
    }else{
        return "";
    }
}

std::string Parser::comp()
{
    if (dest()=="null") {
        if ( command.find(";") == std::string::npos){
            return command;
        }else{
            return command.substr(0, command.find(";"));
        }
    }else if (!(dest()=="")) {
        if ( command.find(";") == std::string::npos){
            return command.substr(dest().length()+1, command.length()-dest().length()-1);
        }else{
            return command.substr(dest().length()+1, command.find(";")-dest().length()-1);
        }
    }else{
        return "";
    }
}

std::string Parser::jump()
{
    if ( command_type == C_COMMAND ){
        if (command.find(";") != std::string::npos){
            return command.substr(command.find(";")+1, command.length()-command.find(";")-1);
        }else{
            return "null";
        }
    }else{
        return "";
    }
}

void Parser::rewind()
{
    ifs.clear();
    ifs.seekg(0, std::ios_base::beg);
}
