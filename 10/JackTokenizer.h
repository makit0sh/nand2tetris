#ifndef JACKTOKENIZER_H_
#define JACKTOKENIZER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>

enum TokenType
{
    KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST
};

enum KeyWord
{
    CLASS,CONSTRUCTOR,FUNCTION,METHOD,FIELD,STATIC,VAR,INT,CHAR,BOOLEAN,VOID,TRUE,FALSE,NULL_K,THIS,LET,DO,IF,ELSE,WHILE,RETURN
};

class JackTokenizer {
    private:
        std::ifstream ifs;
        TokenType token_type;
        std::string token;
        std::vector<std::string> token_list;
    public:
        JackTokenizer(std::string filename);
        ~JackTokenizer();

        bool hasMoreTokens();
        void advance();
        TokenType tokenType();
        void rewind();
        std::string keyWord();
        char symbol();
        std::string identifier();
        int intVal();
        std::string stringVal();

        void debug_xml_output(std::string fileName);
};

#endif // JACKTOKENIZER_H_
