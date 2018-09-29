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
static const regex CLEAR(R"(^\s*(?:c|clear)\s*$)", icase | optimize);
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

    cout << "Interactive AVL Tree" << endl;
    cout << endl;

    cout << "i|insert x                     => Inserts X into the tree" << endl;
    cout << "r|remove x                     => Removes X from the tree" << endl;
    cout << "p|print [(in|pre|post|level)]  => Prints out the tree" << endl;
    cout << "c|clear                        => Clears the tree" << endl;
    cout << "q|e|quit|exit                  => Quits" << endl;
    cout << endl;

    cout << "Have fun!" << endl;

    for (;;) {
        cout << "avl (" << tree.size() << ")> ";
        getline(cin, line);

        if (regex_match(line, QUIT)) {
            break;

        } else if (regex_search(line, m, INSERT)) {
            int n = stoi(m[1]);

            try {
                tree.insert(n);
            } catch (const char* msg) {
                cerr << "Err: " << msg << endl;
            }

        } else if (regex_search(line, m, REMOVE)) {
            int n = stoi(m[1]);
            
            try {
                tree.remove(n);
            } catch (const char* msg) {
                cerr << "Err: " << msg << endl;
            }

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

                for (auto it = tree.level_begin(); it != tree.level_end(); it++) {
                    if (level != it.level()) {
                        cout << endl;
                        level = it.level();
                    }

                    cout << *it << " ";
                }

                cout << endl;

            } else
                cerr << "Err: Invalid printing mode `" << mode << "'" << endl;

        } else if (regex_match(line, CLEAR)) {
            tree.clear();

        } else {
            cerr << "Err: Invalid command" << endl;
            continue;
        }
    }

    return 0;
}