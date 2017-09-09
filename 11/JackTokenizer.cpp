#include "JackTokenizer.h"

JackTokenizer::JackTokenizer(std::string filename) : ifs(filename)
{
    ifs.exceptions(std::ifstream::failbit);
    std::cout << "compiling file: " << filename << std::endl;
}

JackTokenizer::~JackTokenizer()
{
    ifs.close();
}

bool JackTokenizer::hasMoreTokens()
{
    std::string tmp;
    int original_pos = ifs.tellg();
    while (ifs >> tmp) {
        if (tmp.size()>=2 && (tmp.substr(0,2)=="//")) {
            std::getline(ifs, tmp);
        }else if (tmp.size()>=2 && (tmp.substr(0,2)=="/*")) {
            while(true){
                if(ifs.get()=='*') {
                    if (ifs.peek() == '/') {
                        ifs.get();
                        break;
                    }
                }
            }
        }else{
            ifs.seekg(original_pos, std::ios_base::beg);
            return true;
        }
    }
    ifs.seekg(original_pos, std::ios_base::beg);
    return false;
}

void JackTokenizer::advance()
{
    if (!hasMoreTokens()) {
        return;
    }

    char ctmp;
    token = "";

    while (ifs.good()) {
        ifs >> ctmp;
        if (ctmp == '/') {
            if (ifs.peek() == '/') {
                ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }else if (ifs.peek() == '*') {
                ifs.get();
                while(true){
                    if(ifs.get()=='*') {
                        if (ifs.peek() == '/') {
                            ifs.get();
                            break;
                        }
                    }
                }
            }else{
                break;
            }
        }else{
            break;
        }
    }

    if (ctmp == '{' || ctmp == '}' || ctmp == '(' || ctmp == ')' || ctmp == '[' || ctmp == ']'
        || ctmp == '.' || ctmp == ',' || ctmp == ';' || ctmp == '+' || ctmp == '-' || ctmp == '*'
        || ctmp == '/' || ctmp == '&' || ctmp == '|' || ctmp == '<' || ctmp == '>' || ctmp == '=' || ctmp == '~') { 
        token += ctmp;
        token_type = SYMBOL;
    }else if (ctmp=='"') {
        token_type = STRING_CONST;
        while (ifs.good()) {
            if ((ctmp = ifs.get()) == '\n') {
                throw std::runtime_error("STRING_CONST must not contain newline");
            }else if (ctmp == '"') {
                break;
            }else{
                token += ctmp;
            }
        }
    }else if ( '0' <=ctmp && ctmp <= '9' ) {
        token += ctmp;
        token_type = INT_CONST;
        while (ifs.good()){
            if ('0' <= ifs.peek() && ifs.peek() <= '9') {
                token += ifs.get();
            }else{
                break;
            }
        }
    }else if ( ('a'<=ctmp && ctmp<='z') || ('A'<=ctmp && ctmp<='Z') || ctmp=='_' ) {
        token += ctmp;
        while (ifs.good()){
            ctmp = ifs.peek();
            if (('a'<=ctmp && ctmp<='z') || ('A'<=ctmp && ctmp<='Z') || ('0'<=ctmp && ctmp<='9') || ctmp=='_') {
                token += ifs.get();
            }else{
                break;
            }
        }
        if(token == "class" || token == "constructor" || token == "function" || token == "method" ||
            token == "field" || token == "static" || token == "var" || token == "int" || token == "char" ||
            token == "boolean" || token == "void" || token == "true" ||  token == "false" ||   token == "null" ||
            token == "this" || token == "let" || token == "do" || token == "if" || token == "else" ||
            token == "while" || token == "return" ) { 
            token_type = KEYWORD; 
        }else{
            token_type = IDENTIFIER;
        }
    }
}

std::string JackTokenizer::peekToken()
{
    TokenType tmp_token_type = token_type;
    std::string tmp_token = token;
    int original_pos = ifs.tellg();
    advance();
    std::string ans = token;
    token_type = tmp_token_type;
    token = tmp_token;
    ifs.seekg(original_pos, std::ios_base::beg);
    return ans;
}

TokenType JackTokenizer::peekType()
{
    TokenType tmp_token_type = token_type;
    std::string tmp_token = token;
    int original_pos = ifs.tellg();
    advance();
    TokenType ans = token_type;
    token_type = tmp_token_type;
    token = tmp_token;
    ifs.seekg(original_pos, std::ios_base::beg);
    return ans;
}

TokenType JackTokenizer::tokenType()
{
    return token_type;
}

void JackTokenizer::rewind()
{
    ifs.clear();
    ifs.seekg(0, std::ios_base::beg);
}

std::string JackTokenizer::keyWord()
{
    if (token_type==KEYWORD) {
        return token;
    }else{
        throw std::runtime_error("current token is not KEYWORD");
    }
}

char JackTokenizer::symbol()
{
    if (token_type == SYMBOL) {
        return token[0];
    }else{
        throw std::runtime_error("current token is not SYMBOL");
    }
}

std::string JackTokenizer::identifier()
{
    if (token_type == IDENTIFIER) {
        return token;
    }else{
        throw std::runtime_error("current token is not IDENTIFIER");
    }
}

int JackTokenizer::intVal()
{
    if (token_type == INT_CONST) {
        return stoi(token);
    }else{
        throw std::runtime_error("current token is not INT_CONST");
    }
}

std::string JackTokenizer::stringVal()
{
    if (token_type == STRING_CONST) {
        return token;
    }else{
        throw std::runtime_error("current token is not STRING_CONST");
    }
}

void JackTokenizer::xml_output(std::string fileName)
{
    std::ofstream ofs(fileName);
    ofs.exceptions(std::ifstream::failbit);
    ofs << "<tokens>" << std::endl;
    while (hasMoreTokens()){
        advance();
        if (tokenType() == KEYWORD) {
            ofs << "<keyword> " << token << " </keyword>" << std::endl;
        }else if (tokenType() == SYMBOL){
            if (token=="<") {
                ofs << "<symbol> &lt; </symbol>" << std::endl;
            }else if (token==">") {
                ofs << "<symbol> &gt; </symbol>" << std::endl;
            }else if (token=="&") {
                ofs << "<symbol> &amp; </symbol>" << std::endl;
            }else{
                ofs << "<symbol> " << token << " </symbol>" << std::endl;
            }
        }else if (tokenType() == INT_CONST){
            ofs << "<integerConstant> " << token << " </integerConstant>" << std::endl;
        }else if (tokenType() == STRING_CONST){
            ofs << "<stringConstant> " << token << " </stringConstant>" << std::endl;
        }else if (tokenType() == IDENTIFIER){
            ofs << "<identifier> " << token << " </identifier>" << std::endl;
        }
    }
    ofs << "</tokens>" << std::endl;
}

