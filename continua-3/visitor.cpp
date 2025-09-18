#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include "ast.h"
#include "visitor.h"


using namespace std;
unordered_map<std::string, int> memoria;
///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int SqrtExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int RandExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int MinExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int PrintStm::accept(Visitor* visitor) {
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

int PrintVisitor::visit(RandExp* exp) {
    cout << "rand(";
    exp->value1->accept(this);
    cout << ", ";
    exp->value2->accept(this);
    cout <<  ")";
    return 0;
}

int PrintVisitor::visit(MinExp* exp) {
    cout << "min(";
    for (auto e: exp->valuelist)
    {
        e->accept(this);
    }
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

int EVALVisitor::visit(RandExp* exp) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> distribution(exp->value1->accept(this), exp->value2->accept(this));

    int random_number = distribution(generator);

    return random_number;
}


int EVALVisitor::visit(MinExp* exp) {

    int mini = INT32_MAX;

    for(auto e: exp->valuelist)
    {
        int num = e->accept(this);
        mini = min(mini, num);
    }

    return mini;
}

void EVALVisitor::interprete(Program* programa){
    if (programa)
    {
        cout << "Interprete:";
        programa->accept(this);
        cout<<endl;
    }
    return;

}

///////////////////////////////////////////////////////////////////////

int EVALVisitor::visit(PrintStm* stm) {
    // imprimir el primero
    cout << stm->e->accept(this);
    for (size_t i = 0; i < stm->evector.size(); ++i) {
        cout << " " << stm->evector[i]->accept(this);
    }
    cout << endl;
    return 0;
}


int EVALVisitor::visit(AssignStm* stm) {
    memoria[stm->id] = stm->rhs->accept(this);
    return 0;
}
int EVALVisitor::visit(IdExp* exp) {
    return memoria[exp->value];
}

int PrintVisitor::visit(IdExp* exp) {
    cout << exp->value;
    return 0;
}
int EVALVisitor::visit(Program* p) {
    for(auto i:p->slist) {
        i->accept(this);
    }
    return 0;
}

int PrintVisitor::visit(PrintStm* stm) {
    cout << "print(";
    // primero
    stm->e->accept(this);
    // resto de argumentos
    for (size_t i = 0; i < stm->evector.size(); ++i) {
        cout << ", ";
        stm->evector[i]->accept(this);
    }
    cout << ")" << endl;
    return 0;
}


int PrintVisitor::visit(AssignStm* stm) {
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << endl;
    return 0;
}

int PrintVisitor::visit(Program* p) {
    for(auto i:p->slist) {
        i->accept(this);
    }
    return 0;
}