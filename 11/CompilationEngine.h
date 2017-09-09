#ifndef COMPILATIONENGINE_H_
#define COMPILATIONENGINE_H_

#include <iostream>
#include <fstream>
#include <string>

#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"

class CompilationEngine {
    private:
        JackTokenizer& tokenizer;
        SymbolTable symbolTable;
        VMWriter writer;
        std::string className;

        int currentLabelNumber;
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
        int compileExpressionList();

    private:
        bool isOp(char c);
        bool isUnaryOp(char c);
        bool isKeywordConstant(std::string s);
        bool hasMoreTerms();
};

#endif // COMPILATIONENGINE_H_
