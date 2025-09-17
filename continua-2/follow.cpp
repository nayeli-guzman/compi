#include "follow.h"
#include "utils.h"
#include <iostream>
#include <vector>

using namespace std;

/**
 * compute()
 * Calcula los conjuntos FOLLOW para todos los no terminales de la gramática.
 * Algoritmo iterativo hasta que los conjuntos FOLLOW dejen de cambiar.
 */

void Follow::compute() {
    // 1. Inicializar conjuntos FOLLOW vacíos para cada no terminal
    for (const string& nt : grammar.nonTerminals) {
        followSets[nt] = set<string>();
    }

    // =============================
    // Regla 1: El símbolo inicial contiene el EOF '$'
    // =============================
    followSets[grammar.initialState].insert("$");

    bool changed = true;

    while (changed) {
        changed = false;

        // 2. Recorrer todas las reglas de la gramática
        for (const string& r : grammar.rules) {
            string line = trim(r);
            if (line.empty()) continue;

            size_t pos = line.find("->");
            if (pos == string::npos) continue;

            string left = trim(line.substr(0, pos));   // LHS: B
            string right = trim(line.substr(pos + 2)); // RHS: α A γ
            vector<string> alternatives = split(right, '|');

            for (auto& alt : alternatives) {
                vector<string> symbols = split(trim(alt), ' ');

                // 3. Analizar cada símbolo A en la alternativa
                for (size_t i = 0; i < symbols.size(); i++) {
                    string A = trim(symbols[i]);
                    if (A.empty() || !grammar.nonTerminals.count(A)) continue;

                    // =============================
                    // Regla 2: Si existe γ a la derecha de A, agregar FIRST(γ) - {ε} a FOLLOW(A)
                    // =============================
                    if (i + 1 < symbols.size()) {
                        string nextSym = trim(symbols[i + 1]);
                        set<string> firstNext;

                        if (nextSym.front() == '\'' && nextSym.back() == '\'') {
                            // Terminal explícito
                            firstNext.insert(nextSym.substr(1, nextSym.size() - 2));
                        } else if (grammar.nonTerminals.count(nextSym)) {
                            // No terminal
                            firstNext = first.firstSets.at(nextSym); // Usar FIRST calculado
                        } else {
                            // Terminal implícito
                            firstNext.insert(nextSym);
                        }

                        // Insertar FIRST(nextSym) en FOLLOW(A), ignorando ε
                        size_t before = followSets[A].size();
                        for (const string& s : firstNext) {
                            if (s != "''") followSets[A].insert(s);
                        }
                        if (followSets[A].size() > before) changed = true;
                    }
                }
            }
        }
    }
}

/**
 * print()
 * Muestra en consola los conjuntos FOLLOW de cada no terminal
 */
void Follow::print() {
    for (auto& p : followSets) {
        cout << "Follow(" << p.first << ") = { ";
        size_t count = 0;
        for (auto& s : p.second) {
            cout << s;
            count++;
            if (count < p.second.size()) cout << ", ";
        }
        cout << " }" << endl;
    }
}
