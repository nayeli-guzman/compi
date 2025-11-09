#include <iostream>
#include "ast.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;
unordered_map<std::string, int> memoria;
///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int PrintStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int WhileStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}


int IfStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
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

int PrintVisitor::visit(IdExp* exp) {
    cout << exp->value;
    return 0;
}

void PrintVisitor::visit(AssignStm* stm) {
    cout << stm->id << " = ";
    stm->e->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrintStm* stm) {
    cout << "print(";
    stm->e->accept(this);
    cout << ");";
}

void PrintVisitor::visit(IfStm* stm) {
    cout << "if ";
    stm->condicion->accept(this);
    cout << " then" << endl;

    for (auto s : stm->slist1) {
        cout << "    ";
        s->accept(this);
        cout << ";" << endl;
    }

    if (!stm->slist2.empty()) {
        cout << "else" << endl;
        for (auto s : stm->slist2) {
            cout << "    ";
            s->accept(this);
            cout << ";" << endl;
        }
    }

    cout << "endif";
}


void PrintVisitor::visit(WhileStm* stm) {
    cout << "while ";
    stm->condicion->accept(this);
    cout << " do" << endl;

    for (auto s : stm->stlist) {
        cout << "    ";
        s->accept(this);
        cout << ";" << endl;
    }

    cout << "endwhile";
}


void PrintVisitor::imprimir(Program* program){
    for (Stm* s : program->slist) {
        s->accept(this);
        cout << endl;
    }
};

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    int result = 0;

    switch (exp->op) {
        case PLUS_OP:  result = v1 + v2; break;
        case MINUS_OP: result = v1 - v2; break;
        case MUL_OP:   result = v1 * v2; break;
        case DIV_OP:
            if (v2 != 0) result = v1 / v2;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case LE_OP: result = (v1 < v2); break;
        case GR_OP: result = (v1 > v2); break;
        case EQ_OP: result = (v1 == v2); break;
        default:
            cout << "Operador desconocido" << endl;
            break;
    }

    return result;
}

int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

void EVALVisitor::visit(WhileStm* stm) {
    while (stm->condicion->accept(this)) {
        for (auto s : stm->stlist) {
            s->accept(this);
        }
    }
}


int EVALVisitor::visit(IdExp* exp) {
    return memoria[exp->value];
}

void EVALVisitor::visit(AssignStm* stm) {
    memoria[stm->id] = stm->e->accept(this);
}


void EVALVisitor::visit(IfStm* stm) {
    int cond = stm->condicion->accept(this);
    if (cond) {
        for (auto s : stm->slist1)
            s->accept(this);
    } else {
        for (auto s : stm->slist2)
            s->accept(this);
    }
}


void EVALVisitor::visit(PrintStm* stm) {
    cout << stm->e->accept(this);
}
void EVALVisitor::interprete(Program* program){
    for (Stm* s : program->slist) {
        s->accept(this);
    }
};

//////////////////////////////////////
int GencodeVisitor::visit(BinaryExp* exp) {
    switch(exp->op) {
        case PLUS_OP: 
        exp->left->accept(this);
        cout << "pushq %rax" << endl; 
        exp->right->accept(this);
        cout << "movq %rax, %rcx" << endl;
        cout << "popq %rax" << endl;
        cout << "addq %rcx, %rax" << endl;
        break;
        case LE_OP: 
        exp->left->accept(this);
        cout << "pushq %rax" << endl; 
        exp->right->accept(this);
        cout << "movq %rax, %rcx" << endl;
        cout << "popq %rax" << endl;
        cout << "cmpq %rcx, %rax" << endl;
        cout << "movl $0, %eax"<< endl;
        cout << "setl %al" << endl;
        cout << "movzbq %al, %rax" << endl; // deja en el rax 1 si es true , 0 caso contrario
        break;
        case GR_OP:
        exp->left->accept(this);
        cout << "pushq %rax" << endl; 
        exp->right->accept(this);
        cout << "movq %rax, %rcx" << endl;
        cout << "popq %rax" << endl;
        cout << "cmpq %rax, %rcx" << endl;
        cout << "movl $0, %eax" << endl;
        cout << "setg %al" << endl;
        cout << "movzbq %al, %rax" << endl; 
        break;
        case EQ_OP:
        exp->left->accept(this);
        cout << "pushq %rax" << endl; 
        exp->right->accept(this);
        cout << "movq %rax, %rcx" << endl;
        cout << "popq %rax" << endl;
        cout << "cmpq %rcx, %rax" << endl;
        cout << "movl $0, %eax" << endl;
        cout << "sete %al" << endl;
        cout << "movzbq %al, %rax" << endl;
        break;


        default:
            cout << "Operador desconocido" << endl;


    }

    return 0;
}

void GencodeVisitor::visit(WhileStm* stm) {
    int label = labelcont++;
    cout << "while_" << label << ":" << endl;
    stm->condicion->accept(this);
    cout << " cmpq $0, %rax" << endl;
    cout << " je endwhile_" << label << endl;
    for (auto s : stm->stlist) s->accept(this);
    cout << " jmp while_" << label << endl;
    cout << "endwhile_" << label << ":" << endl;
}

int GencodeVisitor::visit(NumberExp* exp) {
    cout << "movq $" << exp->value << ", %rax" << endl; 
    return 0;
}

int GencodeVisitor::visit(IdExp* exp) {
    cout << "movq "<< -8*env[exp->value] << "(%rbp), %rax" << endl;
    return 0;
}


void GencodeVisitor::visit(AssignStm* stm) {
    if (env.count(stm->id))
    {
        stm->e->accept(this);
        cout << "movq %rax ," << -8*env[stm->id] << "(%rbp)" << endl;
    }
    else{
        env[stm->id] = contador;
        stm->e->accept(this);
        cout << "movq %rax ," << -8*env[stm->id] << "(%rbp)" << endl;
        contador++;
    }
}

void GencodeVisitor::visit(PrintStm* stm) {
    stm->e->accept(this);
    cout << "movq %rax, %rsi" << endl; 
    cout << "leaq print_fmt(%rip), %rdi " << endl;
    cout << "movl $0, %eax " << endl;
    cout << "call printf@PLT" << endl;
}

void GencodeVisitor::visit(IfStm* stm) {
    int label = labelcont++;
    stm->condicion->accept(this);
    cout << "cmpq $0, %rax" << endl;
    cout << "je else_" << label << endl;
    for (auto i : stm->slist1){
        i->accept(this);
    }
    cout << "jmp endif_" << label << endl;
    cout << "else_" << label << ":" << endl;
    for (auto i : stm->slist2){
        i->accept(this);
    }
    cout << "endif_" << label << ":" << endl;
}

void GencodeVisitor::code(Program* program){
    cout << ".data" << endl;
    cout << "print_fmt: .string \"%ld\\n\" " << endl;
    cout << ".text "<< endl;
    cout << ".globl main "<< endl;
    cout << "main:"<< endl;
    cout << "pushq %rbp "<< endl;
    cout << "movq %rsp, %rbp "<< endl;
    cout << "subq $8, %rsp "<< endl;
    for (auto i:program->slist)
    {
        i->accept(this);
    }
    cout << "leave"<< endl;
    cout << "ret"<< endl;
    cout << ".section .note.GNU-stack,\"\",@progbits"<< endl;
};


