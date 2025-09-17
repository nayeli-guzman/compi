#include "first.h"
#include "utils.h"
#include <iostream>
#include <vector>

using namespace std;

/**
 * compute()
 * Calcula los conjuntos FIRST para todos los no terminales de la gramática.
 * Algoritmo iterativo hasta que los conjuntos FIRST dejen de cambiar.
 */

 /*
void First::compute() {
    // 1. Inicializar conjuntos FIRST vacíos para cada no terminal
    for (const string& nt : grammar.nonTerminals) {
        firstSets[nt] = set<string>();
    }

    bool changed = true;

    while (changed) {
        changed = false;

        // 2. Recorrer todas las reglas de la gramática
        for (const string& r : grammar.rules) {
            string line = trim(r);
            if (line.empty()) continue;

            size_t pos = line.find("->");
            if (pos == string::npos) continue;

            string left = trim(line.substr(0, pos));   // LHS: no terminal
            string right = trim(line.substr(pos + 2)); // RHS: símbolos de derivación

            // 2.1 Separar RHS en alternativas por '|'
            vector<string> alternatives = split(right, '|');

            for (auto& alt : alternatives) {
                vector<string> symbols = split(trim(alt), ' ');
                if (symbols.empty()) continue;

                string sym = trim(symbols[0]); // Solo el primer símbolo de la alternativa

                set<string> firstSym;

                if (sym.empty()) continue;

                // =============================
                // Regla 1: FIRST(X) = {X} si X es terminal
                // =============================
                if (sym.front() == '\'' && sym.back() == '\'') {
                    firstSym.insert(sym.substr(1, sym.size() - 2)); // Tomamos el terminal
                }
                // =============================
                // Regla 2: FIRST(X1) - {ε} ⊆ FIRST(X)
                // Aquí no consideramos ε, solo propagamos FIRST(X1) tal cual
                // =============================
                else if (grammar.nonTerminals.count(sym)) {
                    firstSym = firstSets[sym]; // Tomamos el FIRST del no terminal X1
                }

                // Caso: terminal implícitp
                else {
                    firstSym.insert(sym); // También aplica regla
                }

                // Insertar los elementos de FIRST(X1) o {X} en FIRST(LHS)
                size_t before = firstSets[left].size();
                firstSets[left].insert(firstSym.begin(), firstSym.end());
                if (firstSets[left].size() > before) changed = true;
            }
        }
    }
}
*/







void First::compute() {
    auto isQuoted = [](const std::string& s) {
        return s.size() >= 2 && s.front()=='\'' && s.back()=='\'';
    };

    const std::string EPS = "ε"; // ajusta si tu gramática usa otro token

    // Inicializar FIRST(NT) = ∅
    for (const string& nt : grammar.nonTerminals) {
        firstSets[nt] = set<string>();
    }

    // Función auxiliar: FIRST de un símbolo individual (terminal o no terminal)
    auto firstOfSymbol = [&](const std::string& sym)->set<string> {
        set<string> out;
        if (sym.empty()) return out;

        // Terminal entre comillas:  'a'  -> a
        if (isQuoted(sym)) {
            string t = sym.substr(1, sym.size()-2);
            if (!t.empty()) out.insert(t);
            return out;
        }

        // Epsilon explícito
        if (sym == EPS || sym == "eps" || sym == "epsilon") {
            out.insert(EPS);
            return out;
        }

        // No terminal
        if (grammar.nonTerminals.count(sym)) {
            return firstSets[sym]; // puede contener ε
        }

        // Terminal “implícito” (sin comillas)
        out.insert(sym);
        return out;
    };

    bool changed = true;
    while (changed) {
        changed = false;

        for (const string& r : grammar.rules) {
            string line = trim(r);
            if (line.empty()) continue;

            size_t pos = line.find("->");
            if (pos == string::npos) continue;

            string left  = trim(line.substr(0, pos));
            string right = trim(line.substr(pos + 2));

            // Lado izquierdo debe ser un no terminal conocido
            if (!grammar.nonTerminals.count(left)) continue;

            // Separar alternativas por '|'
            vector<string> alternatives = split(right, '|');

            for (auto& alt : alternatives) {
                vector<string> symbols = split(trim(alt), ' ');
                if (symbols.empty()) continue;

                bool allNullable = true;

                // Recorremos X1, X2, ..., Xn
                for (size_t i = 0; i < symbols.size(); ++i) {
                    string Xi = trim(symbols[i]);
                    if (Xi.empty()) { allNullable = false; break; }

                    set<string> Fi = firstOfSymbol(Xi);

                    // Regla: FIRST(Xi) - {ε} ⊆ FIRST(left)
                    size_t before = firstSets[left].size();
                    for (const auto& tok : Fi) {
                        if (tok != EPS) firstSets[left].insert(tok);
                    }
                    if (firstSets[left].size() > before) changed = true;

                    // Si ε ∈ FIRST(Xi) seguimos con Xi+1; si no, cortamos.
                    if (Fi.count(EPS) == 0) {
                        allNullable = false;
                        break;
                    }
                }

                // Si todos los Xi pueden derivar ε, añadimos ε a FIRST(left)
                if (allNullable) {
                    if (firstSets[left].insert(EPS).second) changed = true;
                }
            }
        }
    }
}




/**
 * print()
 * Muestra en consola los conjuntos FIRST de cada no terminal
 */
void First::print() {
    for (auto& p : firstSets) {
        cout << "First(" << p.first << ") = { ";
        size_t count = 0;
        for (const auto& s : p.second) {
            cout << s;
            count++;
            if (count < p.second.size()) cout << ", ";
        }
        cout << " }" << endl;
    }
}
