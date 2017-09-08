#ifndef COMPILATIONENGINE_H_
#define COMPILATIONENGINE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <boost/property_tree/xml_parser.hpp>

#include "JackTokenizer.h"


class CompilationEngine {
    private:
        JackTokenizer& tokenizer;
        std::ofstream ofs;
    public:
        CompilationEngine(JackTokenizer& tokenizer, std::string outputFileName);
        ~CompilationEngine();

        void compileClass();
        void compileClassVarDec();
        void compileSubroutine();
        void compileParameterList();
        void compileVarDec();
        void compileStatements();
        void compileDo();
        void compileLet();
        void compileWhile();
        void compileReturn();
        void compileIf();

        void compileExpression();
        void compileTerm();
        void compileExpressionList();
        void compileSubroutineCall();

    private:
        void compileTerminal(std::string type, std::string token);
        void compileTerminal(std::string type, char token);
        void compileTerminal(std::string type, int token);

        bool isOp(char c);
        bool isUnaryOp(char c);
        bool isKeywordConstant(std::string s);
        bool hasMoreTerms();
};

#endif // COMPILATIONENGINE_H_
