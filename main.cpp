#include <algorithm>
#include <iostream>
#include <string>
#include <regex>

#include <avl_tree.hpp>

using namespace std;
using namespace regex_constants;

static const regex INSERT(R"(^\s*(?:i|insert)\s*(\d+)\s*$)", icase | optimize);
static const regex REMOVE(R"(^\s*(?:r|remove)\s*(\d+)\s*$)", icase | optimize);
static const regex PRINT(R"(^\s*(?:p|print)\s*(in|pre|post|level)?\s*$)", icase | optimize);
static const regex CLEAR(R"(^\s*(?:c|r|clear|reset)\s*$)", icase | optimize);
static const regex QUIT(R"(^\s*(?:q|quit|exit)\s*$)", icase | optimize);

/**
 * @brief Ponto de entrada
 * 
 * @param argc Número de argumentos da linha de comando
 * @param argv Valores dos argumentos da linha de comando
 * @return int Código de retorno
 */
int main(int argc, char** argv) {
    string line;
    smatch m;

    avl_tree<int> tree;

    // Cabeçalho
    cout << "Interactive AVL Tree" << endl;
    cout << endl;

    cout << "i|insert x                     : Insert X" << endl;
    cout << "r|remove x                     : Remove X" << endl;
    cout << "p|print [(in|pre|post|level)]  : Print out" << endl;
    cout << "c|r|clear|reset                : Reset" << endl;
    cout << "q|e|quit|exit                  : Quit" << endl;
    cout << endl;

    cout << "Have fun!" << endl;

    for (;;) {

        cout << "avl (" << tree.size() << ")> ";
        getline(cin, line);

        // Encerra o programa
        if (regex_match(line, QUIT)) {
            break;

        // Insere um valor na árvore
        } else if (regex_search(line, m, INSERT)) {
            int n = stoi(m[1]);

            try {
                tree.insert(n);
            } catch (const char* msg) {
                cerr << "Err: " << msg << endl;
            }

        // Remove um valor da árvore
        } else if (regex_search(line, m, REMOVE)) {
            int n = stoi(m[1]);
            
            try {
                tree.remove(n);
            } catch (const char* msg) {
                cerr << "Err: " << msg << endl;
            }

        // Escreve a árvore na tela
        } else if (regex_search(line, m, PRINT)) {
            string mode = m[1];

            if (mode.empty())
                mode = "in";

            transform(
                mode.begin(), mode.end(), 
                mode.begin(), 
                ::tolower
            );

            if (mode == "in") {
                cout << tree << endl;

            } else if (mode == "pre") {


            } else if (mode == "post") {


            } else if (mode == "level") {
                int level = 0;

                for (auto it = tree.begin_by_level(); it != tree.end_by_level(); it++) {
                    if (level != it.level()) {
                        cout << endl;
                        level = it.level();
                    }

                    cout << *it << " ";
                }

                cout << endl;

            } else
                cerr << "Err: Invalid printing mode `" << mode << "'" << endl;

        // Limpa a árvore
        } else if (regex_match(line, CLEAR)) {
            tree.clear();

        // Comando inválido
        } else {
            cerr << "Err: Invalid command" << endl;
            continue;
        }
    }

    return 0;
}