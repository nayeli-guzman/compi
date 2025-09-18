#include <iostream>
#include <stdexcept>
#include <vector>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"


using namespace std;

// =============================
// Métodos de la clase Parser
// =============================

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        throw runtime_error("Error léxico");
    }
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;

        if (check(Token::ERR)) {
            throw runtime_error("Error lexico");
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}


// =============================
// Reglas gramaticales
// =============================

Program* Parser::parseProgram() {
    Program * programa = new Program();
    programa->slist.push_back(parseStm());
    while(match(Token::SEMICOL)) {
        programa->slist.push_back(parseStm());
    }

    if (!isAtEnd()) {
        throw runtime_error("Error sintáctico");
    }
    cout << "Parseo exitoso" << endl;
    return programa;
}

//not yet
Stm* Parser::parseStm() {
    Stm* stm;
    Exp* e;
    string nombre;
    if(match(Token::PRINT)) {
        match(Token::LPAREN);
        e = printArg();
        vector<Exp*> list_prints;
        while(match(Token::COMA))
        {
            list_prints.push_back(printArg());
        }

        match(Token::RPAREN);
        return new PrintStm(e, list_prints);
    }
    else if(match(Token::ID)) {
        nombre = previous->text;
        match(Token::ASSIGN);
        e = parseCE();
        return new AssignStm(nombre, e);
    }
    return stm;
}

//done
Exp* Parser::parseCE() {
    Exp* l = parseE();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseE();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

//done
Exp* Parser::parseE() {
    Exp* l = parseT();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else{
            op = DIV_OP;
        }
        Exp* r = parseT();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

// done
Exp* Parser::parseT() {
    Exp* l = parseF();
    if (match(Token::POW)) {
        BinaryOp op = POW_OP;
        Exp* r = parseF();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseF() {
    Exp* e; 
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    } 
    else if (match(Token::LPAREN))
    {
        e = parseCE();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::SQRT))
    {   
        match(Token::LPAREN);
        e = parseCE();
        match(Token::RPAREN);
        return new SqrtExp(e);
    }
    else if (match(Token::ID))
    {
        return new IdExp(previous->text);
    }
    else if (match(Token::MIN))
    {
        match(Token::LPAREN);
        Exp* first = parseCE();           // 1er argumento
        vector<Exp*> list_nums;
        list_nums.push_back(first);       

        while (match(Token::COMA)) {
            list_nums.push_back(parseCE());  // resto de argumentos
        }
        match(Token::RPAREN);
        return new MinExp(list_nums);
    }

    else if (match(Token::RAND))
    {
        match(Token::LPAREN);
        e = parseCE();
        match(Token::COMA);
        Exp* l = parseCE();
        match(Token::RPAREN);
        return new RandExp(e, l);
    }
    else if (match(Token::STRING)) {            
        return new StringExp(previous->text);
    }
    else {
        throw runtime_error("Error sintáctico");
    }
}

Exp* Parser::printArg() {
    Exp* e; 
    if (match(Token::ID))
    {
        return new IdExp(previous->text);
    } else {
        e = parseCE();
    }
        return e;


    //else {
    //    throw runtime_error("Error sintáctico");
    //}
}