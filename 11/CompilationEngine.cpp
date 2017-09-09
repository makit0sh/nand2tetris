#include "CompilationEngine.h"


CompilationEngine::CompilationEngine(JackTokenizer& inputTokenizer, std::string outputFileName)
    : tokenizer(inputTokenizer), writer(outputFileName), symbolTable(), currentLabelNumber(0)
{
    tokenizer.rewind();
}

CompilationEngine::~CompilationEngine()
{
    writer.close();
}

void CompilationEngine::compileClass()
{
    tokenizer.advance();
    if (tokenizer.keyWord() != "class") throw std::runtime_error("file should contain only one class definition");
    //ofs << "<class>" << std::endl;
    //compileTerminal("keyword", "class");

    tokenizer.advance();
    //compileTerminal("identifier", tokenizer.identifier());
    className = tokenizer.identifier();

    tokenizer.advance();
    if (tokenizer.symbol() != '{') throw std::runtime_error("{ not found");
    //compileTerminal("symbol", tokenizer.symbol());

    while (tokenizer.peekToken()== "static" || tokenizer.peekToken()== "field" ) {
        compileClassVarDec();
    }

    while ( tokenizer.peekToken()== "constructor" || tokenizer.peekToken()=="function" || tokenizer.peekToken()=="method") {
        compileSubroutine();
    }

    tokenizer.advance();
    if (tokenizer.symbol() != '}') throw std::runtime_error("} not found");
    //compileTerminal("symbol", tokenizer.symbol());

    //ofs << "</class>" << std::endl;
} 

void CompilationEngine::compileClassVarDec()
{
    //ofs << "<classVarDec>" << std::endl;

    VarKind kind;
    std::string type;

    tokenizer.advance();
    if (tokenizer.keyWord() != "static" && tokenizer.keyWord() != "field") throw std::runtime_error("need to specify field");
    //compileTerminal("keyword", tokenizer.keyWord());
    kind = symbolTable.string2VarKind(tokenizer.keyWord());

    tokenizer.advance();

    if (tokenizer.tokenType()==KEYWORD) { 
        if (tokenizer.keyWord()=="int" || tokenizer.keyWord()=="char" || tokenizer.keyWord()=="boolean") {
            //compileTerminal("keyword", tokenizer.keyWord());
            type = tokenizer.keyWord();
        }else{
            throw std::runtime_error("wrong type found");
        }
    }else if (tokenizer.tokenType()==IDENTIFIER) {
        //compileTerminal("identifier", tokenizer.identifier());
        type = tokenizer.identifier();
    }else{
        throw std::runtime_error("type unspecified");
    }

    tokenizer.advance();
    if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
    //compileTerminal("identifier", tokenizer.identifier());
    symbolTable.define(tokenizer.identifier(), type, kind);

    while ( tokenizer.peekToken() == "," ) {
        tokenizer.advance();
        //compileTerminal("symbol", tokenizer.symbol());
        tokenizer.advance();
        if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
        //compileTerminal("identifier", tokenizer.identifier());
        symbolTable.define(tokenizer.identifier(), type, kind);
    }

    tokenizer.advance();
    if (tokenizer.symbol()!=';') throw std::runtime_error("; not found");
    //compileTerminal("symbol", tokenizer.symbol());

    //ofs << "</classVarDec>" << std::endl;
}

void CompilationEngine::compileSubroutine()
{
    //ofs << "<subroutineDec>" << std::endl;

    symbolTable.startSubroutine();

    std::string subroutineType;
    std::string returnType;
    std::string subroutineName;

    tokenizer.advance();
    if ( tokenizer.keyWord() != "constructor" && tokenizer.keyWord()!="function" && tokenizer.keyWord()!="method" ) throw std::runtime_error("subroutine type not found");
    //compileTerminal("keyword", tokenizer.keyWord());
    subroutineType = tokenizer.keyWord();
    if (subroutineType=="method") {
        symbolTable.define("this", className, ARG);
    }

    tokenizer.advance();
    if (tokenizer.tokenType()==KEYWORD) { 
        if (tokenizer.keyWord()=="void" || tokenizer.keyWord()=="int" || tokenizer.keyWord()=="char" || tokenizer.keyWord()=="boolean") {
            //compileTerminal("keyword", tokenizer.keyWord());
            returnType = tokenizer.keyWord();
        }else{
            throw std::runtime_error("wrong type found");
        }
    }else if (tokenizer.tokenType()==IDENTIFIER) {
        //compileTerminal("identifier", tokenizer.identifier());
        returnType = tokenizer.identifier();
    }else{
        throw std::runtime_error("type unspecified");
    }

    tokenizer.advance();
    if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("subroutineName not found");
    //compileTerminal("identifier", tokenizer.identifier());
    subroutineName = tokenizer.identifier();

    tokenizer.advance();
    if (tokenizer.symbol()!='(') throw std::runtime_error("( not found");
    //compileTerminal("symbol", tokenizer.symbol());

    compileParameterList();

    tokenizer.advance();
    if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
    //compileTerminal("symbol", tokenizer.symbol());

    //ofs << "<subroutineBody>" << std::endl;

    tokenizer.advance();
    if (tokenizer.symbol() != '{') throw std::runtime_error("{ not found");
    //compileTerminal("symbol", tokenizer.symbol());

    while (tokenizer.peekToken()=="var") {
        compileVarDec();
    }

    writer.writeFunction(className+"."+subroutineName, symbolTable.varCount(VAR));
    if (subroutineType=="constructor") {
        writer.writePush("constant", symbolTable.varCount(FIELD));
        writer.writeCall("Memory.alloc", 1);
        writer.writePop("pointer", 0);
    }else if (subroutineType=="function") {

    }else if (subroutineType=="method") {
        writer.writePush("argument", 0);
        writer.writePop("pointer", 0);
    }

    compileStatements();

    tokenizer.advance();
    if (tokenizer.symbol() != '}') throw std::runtime_error("} not found");
    //compileTerminal("symbol", tokenizer.symbol());
    
    //ofs << "</subroutineBody>" << std::endl;

    //ofs << "</subroutineDec>" << std::endl;
}

void CompilationEngine::compileParameterList()
{
    //ofs << "<parameterList>" << std::endl;

    std::string typeName;

    while (tokenizer.peekType()==KEYWORD || tokenizer.peekType()==IDENTIFIER) {
        tokenizer.advance();
        if (tokenizer.tokenType()==KEYWORD) { 
            if (tokenizer.keyWord()=="int" || tokenizer.keyWord()=="char" || tokenizer.keyWord()=="boolean") {
                //compileTerminal("keyword", tokenizer.keyWord());
                typeName = tokenizer.keyWord();
            }else{
                throw std::runtime_error("wrong type found");
            }
        }else if (tokenizer.tokenType()==IDENTIFIER) {
            //compileTerminal("identifier", tokenizer.identifier());
            typeName = tokenizer.keyWord();
        }else{
            throw std::runtime_error("type unspecified");
        }

        tokenizer.advance();
        if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
        //compileTerminal("identifier", tokenizer.identifier());
        symbolTable.define(tokenizer.identifier(), typeName, ARG);

        while ( tokenizer.peekToken() == "," ) {
            tokenizer.advance();
            //compileTerminal("symbol", tokenizer.symbol());

            tokenizer.advance();
            if (tokenizer.tokenType()==KEYWORD) { 
                if (tokenizer.keyWord()=="int" || tokenizer.keyWord()=="char" || tokenizer.keyWord()=="boolean") {
                    //compileTerminal("keyword", tokenizer.keyWord());
                    typeName = tokenizer.keyWord();
                }else{
                    throw std::runtime_error("wrong type found");
                }
            }else if (tokenizer.tokenType()==IDENTIFIER) {
                //compileTerminal("identifier", tokenizer.identifier());
                typeName = tokenizer.keyWord();
            }else{
                throw std::runtime_error("type unspecified");
            }

            tokenizer.advance();
            if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
            //compileTerminal("identifier", tokenizer.identifier());
            symbolTable.define(tokenizer.identifier(), typeName, ARG);
        }
    }

    //ofs << "</parameterList>" << std::endl;
}

void CompilationEngine::compileVarDec()
{
    //ofs << "<varDec>" << std::endl;

    std::string typeName;

    tokenizer.advance();
    if (tokenizer.keyWord()!="var") throw std::runtime_error("keyword var not found");
    //compileTerminal("keyword", tokenizer.keyWord());

    tokenizer.advance();
    if (tokenizer.tokenType()==KEYWORD) { 
        if (tokenizer.keyWord()=="int" || tokenizer.keyWord()=="char" || tokenizer.keyWord()=="boolean") {
            //compileTerminal("keyword", tokenizer.keyWord());
            typeName = tokenizer.keyWord();
        }else{
            throw std::runtime_error("wrong type found");
        }
    }else if (tokenizer.tokenType()==IDENTIFIER) {
        //compileTerminal("identifier", tokenizer.identifier());
        typeName = tokenizer.identifier();
    }else{
        throw std::runtime_error("type unspecified");
    }

    tokenizer.advance();
    if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
    //compileTerminal("identifier", tokenizer.identifier());
    symbolTable.define(tokenizer.identifier(), typeName, VAR);

    while ( tokenizer.peekToken() == "," ) {
        tokenizer.advance();
        //compileTerminal("symbol", tokenizer.symbol());
        tokenizer.advance();
        if (tokenizer.tokenType()!=IDENTIFIER) throw std::runtime_error("varName not found");
        //compileTerminal("identifier", tokenizer.identifier());
        symbolTable.define(tokenizer.identifier(), typeName, VAR);
    }

    tokenizer.advance();
    if (tokenizer.symbol()!=';') throw std::runtime_error("; not found");
    //compileTerminal("symbol", tokenizer.symbol());
    
    //ofs << "</varDec>" << std::endl;
}

void CompilationEngine::compileStatements()
{
    //ofs << "<statements>" << std::endl;

    while (tokenizer.peekToken()=="let" || tokenizer.peekToken()=="if" || tokenizer.peekToken()=="while" || tokenizer.peekToken()=="do" || tokenizer.peekToken()=="return") {
        if (tokenizer.peekToken()=="let") { compileLet(); }
        else if (tokenizer.peekToken()=="if") { compileIf(); }
        else if (tokenizer.peekToken()=="while") { compileWhile(); }
        else if (tokenizer.peekToken()=="do") { compileDo(); }
        else { compileReturn(); }
    }

    //ofs << "</statements>" << std::endl;
}

void CompilationEngine::compileDo()
{
    //ofs << "<doStatement>" << std::endl;

    std::string subroutineName;
    std::string varName;
    int paramNumber;

    tokenizer.advance();
    if (tokenizer.keyWord()!="do") throw std::runtime_error("keyword do not found");
    //compileTerminal("keyword", tokenizer.keyWord());

    //ofs << "<subroutineCall>" << std::endl;
    tokenizer.advance();
    //compileTerminal("identifier", tokenizer.identifier());
    subroutineName = tokenizer.identifier();

    tokenizer.advance();
    if (tokenizer.symbol()=='(') {
        //compileTerminal("symbol", tokenizer.symbol());
        writer.writePush("pointer", 0);

        paramNumber = compileExpressionList();

        tokenizer.advance();
        if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
        //compileTerminal("symbol", tokenizer.symbol());
        writer.writeCall(className+"."+subroutineName, paramNumber+1);
        
    }else if (tokenizer.symbol()=='.') {
        //compileTerminal("symbol", tokenizer.symbol());
        varName = subroutineName;

        tokenizer.advance();
        //compileTerminal("identifier", tokenizer.identifier());
        subroutineName = tokenizer.identifier();

        bool isVar = false;
        if (symbolTable.kindOf(varName)!=NONE) {
            std::string varSegment;
            switch (symbolTable.kindOf(varName)) {
                case STATIC:
                    varSegment = "static";
                    break;
                case FIELD:
                    varSegment = "this";
                    break;
                case ARG:
                    varSegment = "argument";
                    break;
                case VAR:
                    varSegment = "local";
                    break;
                default:
                    throw std::runtime_error("symbol not found");
            }
            writer.writePush(varSegment, symbolTable.indexOf(varName));
            isVar = true;
        }
        tokenizer.advance();
        if (tokenizer.symbol()!='(') throw std::runtime_error("( not found");
        //compileTerminal("symbol", tokenizer.symbol());

        paramNumber = compileExpressionList();

        tokenizer.advance();
        if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
        //compileTerminal("symbol", tokenizer.symbol());
        if (isVar==true) {
            writer.writeCall(symbolTable.typeOf(varName)+"."+subroutineName, paramNumber+1);
        }else{
            writer.writeCall(varName+"."+subroutineName, paramNumber);
        }
    }else{
        throw std::runtime_error("( or . not found");
    }

    writer.writePop("temp", 0);

    //ofs << "</subroutineCall>" << std::endl;

    tokenizer.advance();
    if (tokenizer.symbol()!=';') throw std::runtime_error("; not found");
    //compileTerminal("symbol", tokenizer.symbol());

    //ofs << "</doStatement>" << std::endl;
}

void CompilationEngine::compileLet()
{
    //ofs << "<letStatement>" << std::endl;

    std::string varName;
    std::string varSegment;
    bool isArray = false;

    tokenizer.advance();
    if (tokenizer.keyWord()!="let") throw std::runtime_error("keyword let not found");
    //compileTerminal("keyword", tokenizer.keyWord());

    tokenizer.advance();
    //compileTerminal("identifier", tokenizer.identifier());
    varName = tokenizer.identifier();

    switch (symbolTable.kindOf(varName)) {
        case STATIC:
            varSegment = "static";
            break;
        case FIELD:
            varSegment = "this";
            break;
        case ARG:
            varSegment = "argument";
            break;
        case VAR:
            varSegment = "local";
            break;
        default:
            throw std::runtime_error("symbol not found");
    }

    if (tokenizer.peekToken()=="[") {
        tokenizer.advance();
        //compileTerminal("symbol", tokenizer.symbol());

        compileExpression();
        writer.writePush(varSegment, symbolTable.indexOf(varName));
        writer.writeArithmetic("add");
        writer.writePop("pointer", 1);
        isArray = true;

        tokenizer.advance();
        if (tokenizer.symbol()!=']') throw std::runtime_error("] not found");
        //compileTerminal("symbol", tokenizer.symbol());
    }
    
    tokenizer.advance();
    if (tokenizer.symbol()!='=') throw std::runtime_error("= not found");
    //compileTerminal("symbol", tokenizer.symbol());

    compileExpression();
    if (isArray) {
        writer.writePop("that", 0);
    }else{
        writer.writePop(varSegment, symbolTable.indexOf(varName));
    }

    tokenizer.advance();
    if (tokenizer.symbol()!=';') throw std::runtime_error("; not found");
    //compileTerminal("symbol", tokenizer.symbol());

    //ofs << "</letStatement>" << std::endl;
}


void CompilationEngine::compileWhile()
{
    //ofs << "<whileStatement>" << std::endl;

    int labelNumber = currentLabelNumber;
    currentLabelNumber++;

    tokenizer.advance();
    if (tokenizer.keyWord()!="while") throw std::runtime_error("keyword while not found");
    //compileTerminal("keyword", tokenizer.keyWord());

    tokenizer.advance();
    if (tokenizer.symbol()!='(') throw std::runtime_error("( not found");
    //compileTerminal("symbol", tokenizer.symbol());

    writer.writeLabel("__WHILE_START"+std::to_string(labelNumber));

    compileExpression();
    writer.writeArithmetic("not");
    writer.writeIf("__WHILE_END"+std::to_string(labelNumber));

    tokenizer.advance();
    if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
    //compileTerminal("symbol", tokenizer.symbol());

    tokenizer.advance();
    if (tokenizer.symbol()!='{') throw std::runtime_error("{ not found");
    //compileTerminal("symbol", tokenizer.symbol());

    compileStatements();

    tokenizer.advance();
    if (tokenizer.symbol()!='}') throw std::runtime_error("} not found");
    //compileTerminal("symbol", tokenizer.symbol());
    writer.writeGoto("__WHILE_START"+std::to_string(labelNumber));
    writer.writeLabel("__WHILE_END"+std::to_string(labelNumber));

    //ofs << "</whileStatement>" << std::endl;
}

void CompilationEngine::compileReturn()
{
    //ofs << "<returnStatement>" << std::endl;

    tokenizer.advance();
    if (tokenizer.keyWord()!="return") throw std::runtime_error("keyword return not found");
    //compileTerminal("keyword", tokenizer.keyWord());

    if (tokenizer.peekToken() != ";") {
        compileExpression();
    }else{
        writer.writePush("constant", 0);
    }

    tokenizer.advance();
    if (tokenizer.symbol()!=';') throw std::runtime_error("; not found");
    //compileTerminal("symbol", tokenizer.symbol());
    writer.writeReturn();

    //ofs << "</returnStatement>" << std::endl;
}

void CompilationEngine::compileIf()
{
    //ofs << "<ifStatement>" << std::endl;

    int labelNumber = currentLabelNumber;
    currentLabelNumber++;

    tokenizer.advance();
    if (tokenizer.keyWord()!="if") throw std::runtime_error("keyword if not found");
    //compileTerminal("keyword", tokenizer.keyWord());

    tokenizer.advance();
    if (tokenizer.symbol()!='(') throw std::runtime_error("( not found");
    //compileTerminal("symbol", tokenizer.symbol());

    compileExpression();
    writer.writeArithmetic("not");
    writer.writeIf("__IF_FALSE"+std::to_string(labelNumber));

    tokenizer.advance();
    if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
    //compileTerminal("symbol", tokenizer.symbol());

    tokenizer.advance();
    if (tokenizer.symbol()!='{') throw std::runtime_error("{ not found");
    //compileTerminal("symbol", tokenizer.symbol());

    compileStatements();

    tokenizer.advance();
    if (tokenizer.symbol()!='}') throw std::runtime_error("} not found");
    //compileTerminal("symbol", tokenizer.symbol());
    writer.writeGoto("__IF_END"+std::to_string(labelNumber));
    writer.writeLabel("__IF_FALSE"+std::to_string(labelNumber));

    while(tokenizer.peekToken()=="else"){
        tokenizer.advance();
        //compileTerminal("keyword", tokenizer.keyWord());

        tokenizer.advance();
        if (tokenizer.symbol()!='{') throw std::runtime_error("{ not found");
        //compileTerminal("symbol", tokenizer.symbol());

        compileStatements();

        tokenizer.advance();
        if (tokenizer.symbol()!='}') throw std::runtime_error("} not found");
        //compileTerminal("symbol", tokenizer.symbol());
    }

    writer.writeLabel("__IF_END"+std::to_string(labelNumber));

    //ofs << "</ifStatement>" << std::endl;
}

void CompilationEngine::compileExpression()
{
    //ofs << "<expression>" << std::endl;

    char op;

    compileTerm();

    while (isOp(tokenizer.peekToken()[0])) {
        tokenizer.advance();
        //compileTerminal("symbol", tokenizer.symbol());
        op = tokenizer.symbol();

        compileTerm();

        if (op=='+') { writer.writeArithmetic("add"); }
        else if (op=='-') { writer.writeArithmetic("sub"); }
        else if (op=='*') { writer.writeCall("Math.multiply", 2); }
        else if (op=='/') { writer.writeCall("Math.divide", 2); }
        else if (op=='&') { writer.writeArithmetic("and"); }
        else if (op=='|') { writer.writeArithmetic("or"); }
        else if (op=='<') { writer.writeArithmetic("lt"); }
        else if (op=='>') { writer.writeArithmetic("gt"); }
        else if (op=='=') { writer.writeArithmetic("eq"); }
    }

    //ofs << "</expression>" << std::endl;
}

void CompilationEngine::compileTerm()
{
    //ofs << "<term>" << std::endl;

    tokenizer.advance();
    if (tokenizer.tokenType()==INT_CONST) {
        //compileTerminal("integerConstant", tokenizer.intVal());
        writer.writePush("constant", tokenizer.intVal());
    }else if (tokenizer.tokenType()==STRING_CONST) {
        //compileTerminal("stringConstant", tokenizer.stringVal());
        writer.writePush("constant", tokenizer.stringVal().size());
        writer.writeCall("String.new", 1);
        for (int i=0;i<tokenizer.stringVal().size();i++) {
            writer.writePush("constant", tokenizer.stringVal()[i]);
            writer.writeCall("String.appendChar", 2);
        }

    }else if (tokenizer.tokenType()==IDENTIFIER ) {
        if (tokenizer.peekType() == SYMBOL && ( tokenizer.peekToken()=="[" || tokenizer.peekToken()=="(" || tokenizer.peekToken()==".")) {
            if (tokenizer.peekToken() == "[") {
                //compileTerminal("identifier", tokenizer.identifier());
                std::string varName = tokenizer.identifier();
                std::string varSegment;
                switch (symbolTable.kindOf(varName)) {
                    case STATIC:
                        varSegment = "static";
                        break;
                    case FIELD:
                        varSegment = "this";
                        break;
                    case ARG:
                        varSegment = "argument";
                        break;
                    case VAR:
                        varSegment = "local";
                        break;
                    default:
                        throw std::runtime_error("symbol not found");
                }
                tokenizer.advance();
                //compileTerminal("symbol", tokenizer.symbol());
                compileExpression();
                writer.writePush(varSegment, symbolTable.indexOf(varName));
                writer.writeArithmetic("add");
                writer.writePop("pointer", 1);
                tokenizer.advance();
                if (tokenizer.symbol()!=']') throw std::runtime_error("] not found");
                //compileTerminal("symbol", tokenizer.symbol());
                writer.writePush("that", 0);
            }else if (tokenizer.peekToken() == "(") {
                //ofs << "<subroutineCall>" << std::endl;
                //compileTerminal("identifier", tokenizer.identifier());
                std::string subroutineName = tokenizer.identifier();
                writer.writePush("pointer", 0);
                tokenizer.advance();
                //compileTerminal("symbol", tokenizer.symbol());
                int paramNumber = compileExpressionList();
                tokenizer.advance();
                if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
                //compileTerminal("symbol", tokenizer.symbol());
                writer.writeCall(className+"."+subroutineName, paramNumber+1);
                //ofs << "</subroutineCall>" << std::endl;
            }else if (tokenizer.peekToken() == ".") {
                //ofs << "<subroutineCall>" << std::endl;
                //compileTerminal("identifier", tokenizer.identifier());
                std::string varName = tokenizer.identifier();
                tokenizer.advance();
                //compileTerminal("symbol", tokenizer.symbol());
                tokenizer.advance();
                //compileTerminal("identifier", tokenizer.identifier());
                std::string subroutineName = tokenizer.identifier();
                std::string varSegment;
                bool isVar = false;
                if (symbolTable.kindOf(varName)!=NONE) {
                    std::string varSegment;
                    switch (symbolTable.kindOf(varName)) {
                        case STATIC:
                            varSegment = "static";
                            break;
                        case FIELD:
                            varSegment = "this";
                            break;
                        case ARG:
                            varSegment = "argument";
                            break;
                        case VAR:
                            varSegment = "local";
                            break;
                        default:
                            throw std::runtime_error("symbol not found");
                    }
                    writer.writePush(varSegment, symbolTable.indexOf(varName));
                    isVar = true;
                }
                tokenizer.advance();
                if (tokenizer.symbol()!='(') throw std::runtime_error("( not found");
                //compileTerminal("symbol", tokenizer.symbol());
                int paramNumber = compileExpressionList();
                tokenizer.advance();
                if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
                //compileTerminal("symbol", tokenizer.symbol());
                if (isVar==true) {
                    writer.writeCall(symbolTable.typeOf(varName)+"."+subroutineName, paramNumber+1);
                }else{
                    writer.writeCall(varName+"."+subroutineName, paramNumber);
                }
                //ofs << "</subroutineCall>" << std::endl;
            }
        }else{
            //compileTerminal("identifier", tokenizer.identifier());
            std::string varName = tokenizer.identifier();
            std::string varSegment;
            switch (symbolTable.kindOf(varName)) {
                case STATIC:
                    varSegment = "static";
                    break;
                case FIELD:
                    varSegment = "this";
                    break;
                case ARG:
                    varSegment = "argument";
                    break;
                case VAR:
                    varSegment = "local";
                    break;
                default:
                    throw std::runtime_error("symbol not found");
            }
            writer.writePush(varSegment, symbolTable.indexOf(varName));
        }
    }else if (tokenizer.tokenType()==KEYWORD && isKeywordConstant(tokenizer.keyWord())) {
        //compileTerminal("keyword", tokenizer.keyWord());
        if (tokenizer.keyWord() == "true") { writer.writePush("constant", 0); writer.writeArithmetic("not"); }
        else if (tokenizer.keyWord() == "false") {writer.writePush("constant", 0); }
        else if (tokenizer.keyWord() == "null") { writer.writePush("constant", 0); }
        else { writer.writePush("pointer", 0); }
    }else if (tokenizer.tokenType()==SYMBOL && (tokenizer.symbol()=='(' || isUnaryOp( tokenizer.symbol()) )) {
        if (tokenizer.symbol()=='(') {
            //compileTerminal("symbol", tokenizer.symbol());
            compileExpression();
            tokenizer.advance();
            if (tokenizer.symbol()!=')') throw std::runtime_error(") not found");
            //compileTerminal("symbol", tokenizer.symbol());
        }else{
            //compileTerminal("symbol", tokenizer.symbol());
            char uop = tokenizer.symbol();
            compileTerm();
            if (uop=='-') { writer.writeArithmetic("neg"); }
            else { writer.writeArithmetic("not"); }
        }
    }else{
        throw std::runtime_error("term not found");
    }

    //ofs << "</term>" << std::endl;
}

int CompilationEngine::compileExpressionList()
{
    //ofs << "<expressionList>" << std::endl;

    int expressionNumber = 0;

    if (hasMoreTerms()) {
        compileExpression();
        expressionNumber++;

        while (tokenizer.peekToken()==",") {
            tokenizer.advance();
            //compileTerminal("symbol", tokenizer.symbol());

            compileExpression();
            expressionNumber++;
        }
    }
    //ofs << "</expressionList>" << std::endl;
    return expressionNumber;
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
