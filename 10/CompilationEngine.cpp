#include "CompilationEngine.h"


CompilationEngine::CompilationEngine(JackTokenizer& inputTokenizer, std::string outputFileName)
    : tokenizer(inputTokenizer), ofs(outputFileName)
{
    ofs.exceptions(std::ifstream::failbit);
    std::cout << "compiling to file: "<< outputFileName << std::endl;
    tokenizer.rewind();
}

CompilationEngine::~CompilationEngine()
{
    ofs.close();
}

void CompilationEngine::compileTerminal(std::string type, std::string token)
{
    ofs << "<" << type << "> " << token << " </" << type << ">" << std::endl;
}

void CompilationEngine::compileTerminal(std::string type, char token)
{
    if (token == '<') {
        ofs << "<" << type << "> " << "&lt;" << " </" << type << ">" << std::endl;
    }else if (token == '>') {
        ofs << "<" << type << "> " << "&gt;" << " </" << type << ">" << std::endl;
    }else if (token == '&') {
        ofs << "<" << type << "> " << "&amp;" << " </" << type << ">" << std::endl;
    }else{
        ofs << "<" << type << "> " << token << " </" << type << ">" << std::endl;
    }
}

void CompilationEngine::compileTerminal(std::string type, int token)
{
    ofs << "<" << type << "> " << token << " </" << type << ">" << std::endl;
}

void CompilationEngine::compileClass()
{
    tokenizer.advance();
    if (tokenizer.keyWord() != "class") throw std::runtime_error("file should contain only one class definition");
    ofs << "<class>" << std::endl;
    compileTerminal("keyword", "class");

    tokenizer.advance();
    compileTerminal("identifier", tokenizer.identifier());

    tokenizer.advance();
    if (tokenizer.symbol() != '{') throw std::runtime_error("{ not found");
    compileTerminal("symbol", tokenizer.symbol());

    while (tokenizer.peekToken()== "static" || tokenizer.peekToken()== "field" ) {
        compileClassVarDec();
    }

    while ( tokenizer.peekToken()== "constructor" || tokenizer.peekToken()=="function" || tokenizer.peekToken()=="method") {
        compileSubroutine();
    }

    tokenizer.advance();
    if (tokenizer.symbol() != '}') throw std::runtime_error("} not found");
    compileTerminal("symbol", tokenizer.symbol());

    ofs << "</class>" << std::endl;
} 

void CompilationEngine::compileClassVarDec()
{
    ofs << "<classVarDec>" << std::endl;

    tokenizer.advance();
    if (tokenizer.keyWord() != "static" && tokenizer.keyWord() != "field") throw std::runtime_error("need to specify field");
    compileTerminal("keyword", tokenizer.keyWord());

    tokenizer.advance();

    if (tokenizer.tokenType()==KEYWORD) { 
        if (tokenizer.keyWord()=="int" || tokenizer.keyWord()=="char" || tokenizer.keyWord()=="boolean") {
            compileTerminal("keyword", tokenizer.keyWord());
        }else{
            throw std::runtime_error("wrong type found");
        }
    }else if (tokenizer.tokenType()==IDENTIFIER) {
        compileTerminal("identifier", tokenizer.identifier());
    }else{
        throw std::runtime_error("type unspecified");
    }

    tokenizer.advance();
    if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
    compileTerminal("identifier", tokenizer.identifier());

    while ( tokenizer.peekToken() == "," ) {
        tokenizer.advance();
        compileTerminal("symbol", tokenizer.symbol());
        tokenizer.advance();
        if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
        compileTerminal("identifier", tokenizer.identifier());
    }

    tokenizer.advance();
    if (tokenizer.symbol()!=';') throw std::runtime_error("; not found");
    compileTerminal("symbol", tokenizer.symbol());

    ofs << "</classVarDec>" << std::endl;
}

void CompilationEngine::compileSubroutine()
{
    ofs << "<subroutineDec>" << std::endl;

    tokenizer.advance();
    if ( tokenizer.keyWord() != "constructor" && tokenizer.keyWord()!="function" && tokenizer.keyWord()!="method" ) throw std::runtime_error("subroutine type not found");
    compileTerminal("keyword", tokenizer.keyWord());

    tokenizer.advance();
    if (tokenizer.tokenType()==KEYWORD) { 
        if (tokenizer.keyWord()=="void" || tokenizer.keyWord()=="int" || tokenizer.keyWord()=="char" || tokenizer.keyWord()=="boolean") {
            compileTerminal("keyword", tokenizer.keyWord());
        }else{
            throw std::runtime_error("wrong type found");
        }
    }else if (tokenizer.tokenType()==IDENTIFIER) {
        compileTerminal("identifier", tokenizer.identifier());
    }else{
        throw std::runtime_error("type unspecified");
    }

    tokenizer.advance();
    if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("subroutineName not found");
    compileTerminal("identifier", tokenizer.identifier());

    tokenizer.advance();
    if (tokenizer.symbol()!='(') throw std::runtime_error("( not found");
    compileTerminal("symbol", tokenizer.symbol());

    compileParameterList();

    tokenizer.advance();
    if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
    compileTerminal("symbol", tokenizer.symbol());

    ofs << "<subroutineBody>" << std::endl;

    tokenizer.advance();
    if (tokenizer.symbol() != '{') throw std::runtime_error("{ not found");
    compileTerminal("symbol", tokenizer.symbol());

    while (tokenizer.peekToken()=="var") {
        compileVarDec();
     }

    compileStatements();

    tokenizer.advance();
    if (tokenizer.symbol() != '}') throw std::runtime_error("} not found");
    compileTerminal("symbol", tokenizer.symbol());
    
    ofs << "</subroutineBody>" << std::endl;

    ofs << "</subroutineDec>" << std::endl;
}

void CompilationEngine::compileParameterList()
{
    ofs << "<parameterList>" << std::endl;

    while (tokenizer.peekType()==KEYWORD || tokenizer.peekType()==IDENTIFIER) {
        tokenizer.advance();
        if (tokenizer.tokenType()==KEYWORD) { 
            if (tokenizer.keyWord()=="int" || tokenizer.keyWord()=="char" || tokenizer.keyWord()=="boolean") {
                compileTerminal("keyword", tokenizer.keyWord());
            }else{
                throw std::runtime_error("wrong type found");
            }
        }else if (tokenizer.tokenType()==IDENTIFIER) {
            compileTerminal("identifier", tokenizer.identifier());
        }else{
            throw std::runtime_error("type unspecified");
        }

        tokenizer.advance();
        if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
        compileTerminal("identifier", tokenizer.identifier());

        while ( tokenizer.peekToken() == "," ) {
            tokenizer.advance();
            compileTerminal("symbol", tokenizer.symbol());

            tokenizer.advance();
            if (tokenizer.tokenType()==KEYWORD) { 
                if (tokenizer.keyWord()=="int" || tokenizer.keyWord()=="char" || tokenizer.keyWord()=="boolean") {
                    compileTerminal("keyword", tokenizer.keyWord());
                }else{
                    throw std::runtime_error("wrong type found");
                }
            }else if (tokenizer.tokenType()==IDENTIFIER) {
                compileTerminal("identifier", tokenizer.identifier());
            }else{
                throw std::runtime_error("type unspecified");
            }

            tokenizer.advance();
            if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
            compileTerminal("identifier", tokenizer.identifier());
        }
    }

    ofs << "</parameterList>" << std::endl;
}

void CompilationEngine::compileVarDec()
{
    ofs << "<varDec>" << std::endl;

    tokenizer.advance();
    if (tokenizer.keyWord()!="var") throw std::runtime_error("keyword var not found");
    compileTerminal("keyword", tokenizer.keyWord());

    tokenizer.advance();
    if (tokenizer.tokenType()==KEYWORD) { 
        if (tokenizer.keyWord()=="int" || tokenizer.keyWord()=="char" || tokenizer.keyWord()=="boolean") {
            compileTerminal("keyword", tokenizer.keyWord());
        }else{
            throw std::runtime_error("wrong type found");
        }
    }else if (tokenizer.tokenType()==IDENTIFIER) {
        compileTerminal("identifier", tokenizer.identifier());
    }else{
        throw std::runtime_error("type unspecified");
    }

    tokenizer.advance();
    if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
    compileTerminal("identifier", tokenizer.identifier());

    while ( tokenizer.peekToken() == "," ) {
        tokenizer.advance();
        compileTerminal("symbol", tokenizer.symbol());
        tokenizer.advance();
        if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
        compileTerminal("identifier", tokenizer.identifier());
    }

    tokenizer.advance();
    if (tokenizer.symbol()!=';') throw std::runtime_error("; not found");
    compileTerminal("symbol", tokenizer.symbol());
    
    ofs << "</varDec>" << std::endl;
}

void CompilationEngine::compileStatements()
{
    ofs << "<statements>" << std::endl;

    while (tokenizer.peekToken()=="let" || tokenizer.peekToken()=="if" || tokenizer.peekToken()=="while" || tokenizer.peekToken()=="do" || tokenizer.peekToken()=="return") {
        if (tokenizer.peekToken()=="let") { compileLet(); }
        else if (tokenizer.peekToken()=="if") { compileIf(); }
        else if (tokenizer.peekToken()=="while") { compileWhile(); }
        else if (tokenizer.peekToken()=="do") { compileDo(); }
        else { compileReturn(); }
    }

    ofs << "</statements>" << std::endl;
}

void CompilationEngine::compileDo()
{
    ofs << "<doStatement>" << std::endl;

    tokenizer.advance();
    if (tokenizer.keyWord()!="do") throw std::runtime_error("keyword do not found");
    compileTerminal("keyword", tokenizer.keyWord());

    compileSubroutineCall();

    tokenizer.advance();
    if (tokenizer.symbol()!=';') throw std::runtime_error("; not found");
    compileTerminal("symbol", tokenizer.symbol());

    ofs << "</doStatement>" << std::endl;
}

void CompilationEngine::compileLet()
{
    ofs << "<letStatement>" << std::endl;

    tokenizer.advance();
    if (tokenizer.keyWord()!="let") throw std::runtime_error("keyword let not found");
    compileTerminal("keyword", tokenizer.keyWord());

    tokenizer.advance();
    compileTerminal("identifier", tokenizer.identifier());

    if (tokenizer.peekToken()=="[") {
        tokenizer.advance();
        compileTerminal("symbol", tokenizer.symbol());

        compileExpression();
        
        tokenizer.advance();
        if (tokenizer.symbol()!=']') throw std::runtime_error("] not found");
        compileTerminal("symbol", tokenizer.symbol());
    }
    
    tokenizer.advance();
    if (tokenizer.symbol()!='=') throw std::runtime_error("= not found");
    compileTerminal("symbol", tokenizer.symbol());

    compileExpression();

    tokenizer.advance();
    if (tokenizer.symbol()!=';') throw std::runtime_error("; not found");
    compileTerminal("symbol", tokenizer.symbol());

    ofs << "</letStatement>" << std::endl;
}


void CompilationEngine::compileWhile()
{
    ofs << "<whileStatement>" << std::endl;

    tokenizer.advance();
    if (tokenizer.keyWord()!="while") throw std::runtime_error("keyword while not found");
    compileTerminal("keyword", tokenizer.keyWord());

    tokenizer.advance();
    if (tokenizer.symbol()!='(') throw std::runtime_error("( not found");
    compileTerminal("symbol", tokenizer.symbol());

    compileExpression();

    tokenizer.advance();
    if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
    compileTerminal("symbol", tokenizer.symbol());

    tokenizer.advance();
    if (tokenizer.symbol()!='{') throw std::runtime_error("{ not found");
    compileTerminal("symbol", tokenizer.symbol());

    compileStatements();

    tokenizer.advance();
    if (tokenizer.symbol()!='}') throw std::runtime_error("} not found");
    compileTerminal("symbol", tokenizer.symbol());

    ofs << "</whileStatement>" << std::endl;
}

void CompilationEngine::compileReturn()
{
    ofs << "<returnStatement>" << std::endl;

    tokenizer.advance();
    if (tokenizer.keyWord()!="return") throw std::runtime_error("keyword return not found");
    compileTerminal("keyword", tokenizer.keyWord());

    if (tokenizer.peekToken() != ";") compileExpression();

    tokenizer.advance();
    if (tokenizer.symbol()!=';') throw std::runtime_error("; not found");
    compileTerminal("symbol", tokenizer.symbol());

    ofs << "</returnStatement>" << std::endl;
}

void CompilationEngine::compileIf()
{
    ofs << "<ifStatement>" << std::endl;

    tokenizer.advance();
    if (tokenizer.keyWord()!="if") throw std::runtime_error("keyword if not found");
    compileTerminal("keyword", tokenizer.keyWord());

    tokenizer.advance();
    if (tokenizer.symbol()!='(') throw std::runtime_error("( not found");
    compileTerminal("symbol", tokenizer.symbol());

    compileExpression();

    tokenizer.advance();
    if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
    compileTerminal("symbol", tokenizer.symbol());

    tokenizer.advance();
    if (tokenizer.symbol()!='{') throw std::runtime_error("{ not found");
    compileTerminal("symbol", tokenizer.symbol());

    compileStatements();

    tokenizer.advance();
    if (tokenizer.symbol()!='}') throw std::runtime_error("} not found");
    compileTerminal("symbol", tokenizer.symbol());

    while(tokenizer.peekToken()=="else"){
        tokenizer.advance();
        compileTerminal("keyword", tokenizer.keyWord());

        tokenizer.advance();
        if (tokenizer.symbol()!='{') throw std::runtime_error("{ not found");
        compileTerminal("symbol", tokenizer.symbol());

        compileStatements();

        tokenizer.advance();
        if (tokenizer.symbol()!='}') throw std::runtime_error("} not found");
        compileTerminal("symbol", tokenizer.symbol());
    }

    ofs << "</ifStatement>" << std::endl;
}

void CompilationEngine::compileExpression()
{
    ofs << "<expression>" << std::endl;

    compileTerm();

    while (isOp(tokenizer.peekToken()[0])) {
        tokenizer.advance();
        compileTerminal("symbol", tokenizer.symbol());

        compileTerm();
    }

    ofs << "</expression>" << std::endl;
}

void CompilationEngine::compileTerm()
{
    ofs << "<term>" << std::endl;

    tokenizer.advance();
    if (tokenizer.tokenType()==INT_CONST) {
        compileTerminal("integerConstant", tokenizer.intVal());
    }else if (tokenizer.tokenType()==STRING_CONST) {
        compileTerminal("stringConstant", tokenizer.stringVal());
    }else if (tokenizer.tokenType()==IDENTIFIER ) {
        if (tokenizer.peekType() == SYMBOL && ( tokenizer.peekToken()=="[" || tokenizer.peekToken()=="(" || tokenizer.peekToken()==".")) {
            if (tokenizer.peekToken() == "[") {
                compileTerminal("identifier", tokenizer.identifier());
                tokenizer.advance();
                compileTerminal("symbol", tokenizer.symbol());
                compileExpression();
                tokenizer.advance();
                if (tokenizer.symbol()!=']') throw std::runtime_error("] not found");
                compileTerminal("symbol", tokenizer.symbol());
            }else if (tokenizer.peekToken() == "(") {
                //ofs << "<subroutineCall>" << std::endl;
                compileTerminal("identifier", tokenizer.identifier());
                tokenizer.advance();
                compileTerminal("symbol", tokenizer.symbol());
                compileExpressionList();
                tokenizer.advance();
                if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
                compileTerminal("symbol", tokenizer.symbol());
                //ofs << "</subroutineCall>" << std::endl;
            }else if (tokenizer.peekToken() == ".") {
                //ofs << "<subroutineCall>" << std::endl;
                compileTerminal("identifier", tokenizer.identifier());
                tokenizer.advance();
                compileTerminal("symbol", tokenizer.symbol());
                tokenizer.advance();
                compileTerminal("identifier", tokenizer.identifier());
                tokenizer.advance();
                if (tokenizer.symbol()!='(') throw std::runtime_error("( not found");
                compileTerminal("symbol", tokenizer.symbol());
                compileExpressionList();
                tokenizer.advance();
                if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
                compileTerminal("symbol", tokenizer.symbol());
                //ofs << "</subroutineCall>" << std::endl;
            }
        }else{
            compileTerminal("identifier", tokenizer.identifier());
        }
    }else if (tokenizer.tokenType()==KEYWORD && isKeywordConstant(tokenizer.keyWord())) {
        compileTerminal("keyword", tokenizer.keyWord());
    }else if (tokenizer.tokenType()==SYMBOL && (tokenizer.symbol()=='(' || isUnaryOp( tokenizer.symbol()) )) {
        if (tokenizer.symbol()=='(') {
            compileTerminal("symbol", tokenizer.symbol());
            compileExpression();
            tokenizer.advance();
            if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
            compileTerminal("symbol", tokenizer.symbol());
        }else{
            compileTerminal("symbol", tokenizer.symbol());
            compileTerm();
        }
    }else{
        throw std::runtime_error("term not found");
    }

    ofs << "</term>" << std::endl;
}

void CompilationEngine::compileExpressionList()
{
    ofs << "<expressionList>" << std::endl;

    if (hasMoreTerms()) {
        compileExpression();

        while (tokenizer.peekToken()==",") {
            tokenizer.advance();
            compileTerminal("symbol", tokenizer.symbol());

            compileExpression();
        }
    }

    ofs << "</expressionList>" << std::endl;
}

void CompilationEngine::compileSubroutineCall()
{
    //ofs << "<subroutineCall>" << std::endl;

    tokenizer.advance();
    compileTerminal("identifier", tokenizer.identifier());

    tokenizer.advance();
    if (tokenizer.symbol()=='(') {
        compileTerminal("symbol", tokenizer.symbol());

        compileExpressionList();

        tokenizer.advance();
        if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
        compileTerminal("symbol", tokenizer.symbol());

    }else if (tokenizer.symbol()=='.') {
        compileTerminal("symbol", tokenizer.symbol());

        tokenizer.advance();
        compileTerminal("identifier", tokenizer.identifier());

        tokenizer.advance();
        if (tokenizer.symbol()!='(') throw std::runtime_error("( not found");
        compileTerminal("symbol", tokenizer.symbol());

        compileExpressionList();

        tokenizer.advance();
        if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
        compileTerminal("symbol", tokenizer.symbol());

    }else{
        throw std::runtime_error("( or . not found");
    }

    //ofs << "</subroutineCall>" << std::endl;
}

bool CompilationEngine::isOp(char c)
{
    if ( c=='+' || c=='-' || c=='*' || c=='/' || c=='&' || c=='|' || c=='<' || c=='>' || c=='=' ) {
        return true;
    }else{
        return false;
    }
}

bool CompilationEngine::isUnaryOp(char c)
{
    if ( c=='-' || c=='~' ) {
        return true;
    }else{
        return false;
    }
}

bool CompilationEngine::isKeywordConstant(std::string s)
{
    if ( s=="true" || s=="false" || s=="null" || s=="this" ) {
        return true;
    }else{
        return false;
    }
}

bool CompilationEngine::hasMoreTerms()
{
    if (tokenizer.peekType()==INT_CONST || tokenizer.peekType()==STRING_CONST || tokenizer.peekType()==IDENTIFIER ) {
        return true;
    }else if (tokenizer.peekType()==KEYWORD && isKeywordConstant(tokenizer.peekToken())) {
        return true;
    }else if (tokenizer.peekType()==SYMBOL && (tokenizer.peekToken()=="(" || isUnaryOp( tokenizer.peekToken()[0])) ) {
        return true;
    }else{
        return false;
    }
}
