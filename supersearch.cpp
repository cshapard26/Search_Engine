#include <chrono>
#include <cmath>
#include <iostream>

#include "classes/AVLTree.cpp"
#include "classes/DocumentParser.cpp"
#include "classes/IndexHandler.cpp"
#include "classes/QueryEngine.cpp"

int main(int argc, char **argv) {
    if (argc > 3 || argc < 2) {
        std::cout << "Invalid command line arguments. Type './supersearch help' for more information.\n";
        return 1;
    }
    if ((std::strcmp(argv[1], "index") == 0 || std::strcmp(argv[1], "query") == 0) && (argc < 3 || argc > 3)) {
        std::cout << "Invalid command line arguments. Type './supersearch help' for more information.\n";
        return 1;
    }
    if ((std::strcmp(argv[1], "ui") == 0 || std::strcmp(argv[1], "help") == 0) && argc > 2) {
        std::cout << "Invalid command line arguments. Type './supersearch help' for more information.\n";
        return 1;
    }

    AVLTree termTree;
    AVLTree personTree;
    AVLTree orgTree;
    IndexHandler indexHandler = IndexHandler(&termTree, &personTree, &orgTree);
    QueryEngine engine = QueryEngine(indexHandler);

    if (std::strcmp(argv[1], "ui") == 0) {
        bool continuePrompt = true;
        while (continuePrompt) {
            continuePrompt = engine.UI();
        }
    } else if (std::strcmp(argv[1], "index") == 0) {
        engine.addArticles(argv[2]);
        engine.saveToDisk();
    } else if (std::strcmp(argv[1], "query") == 0) {
        engine.loadFromDisk();
        engine.query(argv[2]);
    } else if (std::strcmp(argv[1], "help") == 0) {
        printHeader();
        std::cout << "Commands:\n";
        std::cout << "\tui                          - Guides the user through a clean UI for the program. The prefered way to interact with SuperSearch Engine.\n";
        std::cout << "\tindex <file/directory>      - Indexes all articles in the specified directory and saves it to 'supersearch.corpus'.\n";
        std::cout << "\tquery \"Your Query Terms\"    - Outputs relevant results from 'supersearch.corpus'.\n";
        std::cout << "\thelp                        - Displays this page.\n\n";

    } else {
        std::cout << "Invalid command line arguments. Type './supersearch help' for more information.\n";
        return 1;
    }

    return 0;
}