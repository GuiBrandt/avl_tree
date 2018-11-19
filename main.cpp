#include <algorithm>
#include <iostream>
#include <string>
#include <stack>
#include <regex>

#include <avl_tree.hpp>

using namespace std;
using namespace regex_constants;

static const regex INSERT(R"(^\s*(?:i|insert)\s*(\d+)\s*$)", icase | optimize);
static const regex REMOVE(R"(^\s*(?:r|remove)\s*(\d+)\s*$)", icase | optimize);
static const regex PRINT(R"(^\s*(?:p|print)\s*(in|pre|post|level)?\s*$)", icase | optimize);
static const regex CLEAR(R"(^\s*(?:c|r|clear|reset)\s*$)", icase | optimize);
static const regex SAVE(R"(^\s*(?:s|save)\s+([^\\\?%\*]+)\s*$)", icase | optimize);
static const regex SAVEGV(R"(^\s*(?:g|graphviz)\s+([^\\\?%\*]+)\s*$)", icase | optimize);
static const regex QUIT(R"(^\s*(?:q|quit|exit)\s*$)", icase | optimize);

/**
 * @brief Salva uma árvore num arquivo .gv na linguagem dot
 * 
 * @param file Arquivo de saída
 * @param tree Árvore a ser salva
 * @param i ID do nó no arquivo (gambiarra)
 */
void gv_save(ofstream& file, avl_tree<int>* tree, int& i) {
    if (tree->empty())
        return;

    int current = i;
    file << "node" << current << " [label=" << tree->get_info() << "]" << endl;

    if (tree->get_left()) {
        i++;

        int left = i;
        gv_save(file, tree->get_left(), i);

        file    << "node" << current
                << " -- "
                << "node" << left << endl;
    }

    if (tree->get_right()) {
        i++;

        int right = i;
        gv_save(file, tree->get_right(), i);

        file    << "node" << current
                << " -- " 
                << "node" << right << endl;
    }
}

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

    cout << "i|insert x                 : Insert X" << endl;
    cout << "r|remove x                 : Remove X" << endl;
    cout << "p|print [(sorted|level)]   : Print out" << endl;
    cout << "s|save <filename>          : Save to file" << endl;
    cout << "g|graphviz <filename>      : Save Graphviz model to file" << endl;
    cout << "c|r|clear|reset            : Reset" << endl;
    cout << "q|e|quit|exit              : Quit" << endl;
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
                mode = "sorted";

            transform(
                mode.begin(), mode.end(), 
                mode.begin(), 
                ::tolower
            );

            // Escreve os elementos da árvore em ordem
            if (mode == "sorted") {
                for (auto it = tree.begin_in_order(); it != tree.end_in_order(); it++)
                    cout << *it << " ";

                cout << endl;

            // Escreve os elementos da árvore por nível
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
        
        // Salva a árvore num arquivo
        } else if (regex_search(line, m, SAVE)) {
            ofstream f(m[1], ios::binary);
            f << tree;
            f.close();
            
        // Salva a árvore como modelo do graphviz num arquivo
        } else if (regex_search(line, m, SAVEGV)) {
            ofstream f(m[1]);
            
            f << "strict graph {" << endl;
            f << "node [shape=rect]" << endl;

            int i = 0;
            gv_save(f, &tree, i);

            f << "}";
            
            f.close();
            
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