#include <iostream>
#include <stdexcept>
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

Exp* Parser::parseProgram() {
    Exp* ast = parseA();
    if (!isAtEnd()) {
        throw runtime_error("Error sintáctico");
    }
    cout << "Parseo exitoso" << endl;
    return ast;
}

/*
Exp* Parser::parseP() {
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
*/

Exp* Parser::parseP() {
    Exp* l = parseA();
    return l;
}

Exp* Parser::parseA() {
    Exp* l = parseE();

    if(match(Token::LT)) {
        BinaryOp op = LT_OP;
        Exp* r = parseB();
        l = new BinaryExp(l, r, op);
    }

    return l;
}

Exp* Parser::parseB() {
    Exp* l = parseC();

    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseC();
        l = new BinaryExp(l, r, op);
    }
    

    return l;
}

Exp* Parser::parseC() {
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
        e = parseA();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::SQRT))
    {   
        match(Token::LPAREN);
        e = parseA();
        match(Token::RPAREN);
        return new SqrtExp(e);
    }
    else if (match(Token::MIN))
    {   
        match(Token::LPAREN);
        e = parseA();
        match(Token::COMA);
        Exp* f = parseA();
        match(Token::RPAREN);
        return new BinaryExp(e, f, MIN_OP);
    }
    else if (match(Token::MAX))
    {   
        match(Token::LPAREN);
        e = parseA();
        match(Token::COMA);
        Exp* f = parseA();
        match(Token::RPAREN);
        return new BinaryExp(e, f, MAX_OP);
    }
    else if (match(Token::IF))
    {   
        match(Token::LPAREN);
        e = parseA();
        match(Token::COMA);
        Exp* f = parseA();
        match(Token::COMA);
        Exp* g = parseA();
        match(Token::RPAREN);
        return new IfExp(e, f, g);
    }
    else if (match(Token::MINUS))
    {
        e = parseF();
        return new BinaryExp(new NumberExp(0), e, MINUS_OP);
    }
    else {
        throw runtime_error("Error sintáctico");
    }
}


/*

    Exp* parseP();                   // Regla gramatical P
    Exp* parseA();                   // Regla gramatical A
    Exp* parseB();
    Exp* parseR();
    Exp* parseC();
    Exp* parseE();                   // Regla gramatical E
    Exp* parseT();                   // Regla gramatical T
    Exp* parseF(); 

*/