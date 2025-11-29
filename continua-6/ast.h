#ifndef AST_H
#define AST_H

#include <string>
#include <unordered_map>
#include <list>
#include <ostream>

using namespace std;

class Visitor; 

class Body;

// Operadores binarios soportados
enum BinaryOp { 
    PLUS_OP, 
    MINUS_OP, 
    MUL_OP, 
    DIV_OP,
    POW_OP,
    LE_OP
};

// Clase abstracta Exp
class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;  // Destructor puro → clase abstracta
    static string binopToChar(BinaryOp op);  // Conversión operador → string
};

// Expresión binaria
class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    int accept(Visitor* visitor);
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();

};

// Expresión numérica
class NumberExp : public Exp {
public:
    bool is_bin = false;
    int value;
    int accept(Visitor* visitor);    NumberExp(int v);
    ~NumberExp();
};

// Expresión numérica
class IdExp : public Exp {
public:
    string value;
    int accept(Visitor* visitor);
    IdExp(string v);
    ~IdExp();
};

// Raiz cuadrada
class SqrtExp : public Exp {
public:
    Exp* value;
    int accept(Visitor* visitor);
    SqrtExp(Exp* v);
    ~SqrtExp();
}
;

class Stm{
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};



class IfStm: public Stm {
public:
    Exp* condicion;
    bool parteelse;
    Body* body;
    Body* bodyElse;
    IfStm(Exp*);
    ~IfStm();
    int accept(Visitor* visitor);
};

class WhileStm: public Stm {
public:
    Exp* condicion;
    list<Stm*> slist1;  
    Body* body; 

    WhileStm(Exp* c) : condicion(c), body(nullptr) {}
    ~WhileStm();
    int accept(Visitor* visitor);
};

class AssignStm: public Stm {
public:
    string id;
    Exp* e;
    AssignStm(string, Exp*);
    ~AssignStm();
    int accept(Visitor* visitor);
};
class PrintStm: public Stm {
public:
    Exp* e;
    PrintStm(Exp*);
    ~PrintStm();
    int accept(Visitor* visitor);
};

class VarDec {
public:
    string type;
    list<string> variables; 

    VarDec();
    VarDec(string, list<string>);
    ~VarDec();
    int accept(Visitor* );
};


class Body {
public:
    list<Stm*> slist;
    list<VarDec*> vlist;

    void add(Stm*);
    void add(VarDec*);
    ~Body();
    int accept(Visitor* visitor);

};
class Program{
public:
    Body* body;
    Program();
    ~Program();
    int accept(Visitor* visitor);
};
#endif // AST_H
