#include <iostream>
#include <fstream>
#include <cmath>
#include "ast.h"
#include "visitor.h"
#include "environment.h"

using namespace std;
Environment memoria;

///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int SqrtExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int PrintStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}
int VarDec::accept(Visitor* visitor) {
    return visitor->visit(this);
}
int Body::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IfStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int WhileStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

///////////////////////////////////////////////////////////////////////////////////

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(SqrtExp* exp) {
    cout << "sqrt(";
    exp->value->accept(this);
    cout <<  ")";
    return 0;
}


void PrintVisitor::imprimir(Program* programa){
    if (programa)
    {
        cout << "Codigo:" << endl; 
        programa->accept(this);
        cout << endl;
    }
    return ;
}

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch (exp->op) {
        case LE_OP:
            result = v1 < v2;
            break;
        case PLUS_OP:
            result = v1 + v2;
            break;
        case MINUS_OP:
            result = v1 - v2;
            break;
        case MUL_OP:
            result = v1 * v2;
            break;
        case DIV_OP:
            if (v2 != 0)
                result = v1 / v2;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case POW_OP:
            result = pow(v1,v2);
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(SqrtExp* exp) {
    return floor(sqrt( exp->value->accept(this)));
}

void EVALVisitor::interprete(Program* programa){
    if (programa)
    {
        cout << "Interprete:" << endl;
        programa->accept(this);
    }
    return;

}

int EVALVisitor::visit(VarDec* stm) {
    for (auto e:stm->variables) {
        memoria.add_var(e, stm->type);
    }
    return 0;
}

int EVALVisitor::visit(Body* stm) {
    memoria.add_level();
    for (auto e:stm->vlist) {
        e->accept(this);
    }
    for (auto e:stm->slist) {
        e->accept(this);
    }
    memoria.remove_level();
    return 0;
}


//////////////////////////////////////////

int PrintVisitor::visit(Program* p) {
    p->body->accept(this);
    return 0;
}

int PrintVisitor::visit(Body* p) {
    for(auto i: p->vlist){
        i->accept(this);
    }

    for(auto i: p->slist){
        i->accept(this);
    }
    return 0;
}

int PrintVisitor::visit(PrintStm* stm) {
    cout << "print(";
    stm->e->accept(this);
    cout << ")" << endl;
    return 0;
}

int PrintVisitor::visit(AssignStm* stm) {
    cout << stm->id << "=";
    stm->e->accept(this);
    cout << endl;
    return 0;
}

int PrintVisitor::visit(VarDec* stm) {
    cout << "var " << stm->type << " ";
    for (auto i:stm->variables) {
        cout << i << ", ";
    } cout << endl;
    return 0;
}

int PrintVisitor::visit(IdExp* p) {
    cout << p->value;
    return 0;
}

int PrintVisitor::visit(IfStm* stm) {
    cout << "if " ;
    stm->condicion->accept(this);
    cout  << " then" << endl;
    stm->body->accept(this);
    if (stm->parteelse){
        cout << "else"  << endl;;
        stm->bodyElse->accept(this);
    }   
    cout << "endif" << endl;
    return 0;
}

int PrintVisitor::visit(WhileStm* stm) {
    cout << "while ";
    stm->condicion->accept(this);
    cout << " do" << endl;

    if (stm->body) {
        stm->body->accept(this);   
    }

    cout << "endwhile" << endl;
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////

int EVALVisitor::visit(Program* p) {
    p->body->accept(this);
    return 0;
}

int EVALVisitor::visit(PrintStm* p) {
    cout << p->e->accept(this) << endl; 
    return 0;
}

int EVALVisitor::visit(AssignStm* p) {
    memoria.update(p->id, p->e->accept(this));
    return 0;
}

int EVALVisitor::visit(IdExp* p) {
    return memoria.lookup(p->value);
}

int EVALVisitor::visit(IfStm* stm) {
    if (stm->condicion->accept(this)){
        stm->body->accept(this);
    }
    else{
        stm->bodyElse->accept(this);

    }
    return 0;
}

int EVALVisitor::visit(WhileStm* stm) {
    while (stm->condicion->accept(this)) {
        if (stm->body) {
            stm->body->accept(this);   // ejecuta el Body con su propio scope
        }
    }
    return 0;
}
