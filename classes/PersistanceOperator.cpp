#ifndef PERSISTANCEOPERATORCPP
#define PERSISTANCEOPERATORCPP

#include "PersistanceOperator.h"

PersistanceOperator::PersistanceOperator() {
}

std::vector<std::string> split(const std::string& s, char delim) {  // Credit to Arafat Hasan for this split function (https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c)
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void printHeader() {  // A pretty graphic for the UI
    std::cout << "============================\n";
    std::cout << "==== SuperSearch Engine ====\n";
    std::cout << "============================\n\n";
}

void PersistanceOperator::writeTreeToFile(AVLTree* theTree, std::string fileLocation) {  // Writes all AVLTrees to the specified file. Displays timing.

    /*
        FORMAT:
            ^^ARTICLES IN VECTOR LIST (TITLE; TEXT; PERSONLIST; ORGLIST)
        ---
            --TREE NODES (ID;HEIGHT;ARTICLE LIST BY INDEX)
    */

    std::ofstream output(fileLocation);
    if (!output.is_open()) {
        std::cerr << "Cannot open file: " << fileLocation << std::endl;
        return;
    }
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Setting up file and tree...\n";

    // Writes Articles
    std::set<ArticleObject*> seenArticles = theTree->getArticlesFromTree(theTree->root);
    std::vector<ArticleObject*> theArticles(seenArticles.begin(), seenArticles.end());
    int numArts = 0;
    for (size_t i = 0; i < theArticles.size(); i++) {
        auto temp = theArticles[i];
        std::string theText = temp->articleText;
        size_t pos = temp->articleText.find('\n');

        while (pos != std::string::npos) {
            theText.replace(pos, 1, "~");
            pos = theText.find('\n', pos + 1);
        }

        pos = temp->articleText.find(';');

        while (pos != std::string::npos) {
            theText.replace(pos, 1, "`");
            pos = theText.find(';', pos + 1);
        }

        std::vector<std::string> thePeople = temp->people;
        std::string peopleAndOrgs;
        if (thePeople.size() > 1) {
            for (size_t j = 0; j < thePeople.size() - 1; j++) {
                peopleAndOrgs.append(thePeople[j]);
                peopleAndOrgs.append(",");
            }
            peopleAndOrgs.append(thePeople[thePeople.size() - 1]);
            peopleAndOrgs.append(";");
        } else if (thePeople.size() == 1) {
            peopleAndOrgs.append(thePeople[0]);
            peopleAndOrgs.append(";");
        } else {
            peopleAndOrgs.append(";");
        }

        std::vector<std::string> theOrgs = temp->orgs;
        if (theOrgs.size() > 1) {
            for (size_t j = 0; j < theOrgs.size() - 1; j++) {
                peopleAndOrgs.append(theOrgs[j]);
                peopleAndOrgs.append(",");
            }
            peopleAndOrgs.append(theOrgs[theOrgs.size() - 1]);
            peopleAndOrgs.append(";");
        } else if (theOrgs.size() == 1) {
            peopleAndOrgs.append(theOrgs[0]);
            peopleAndOrgs.append(";");
        } else {
            peopleAndOrgs.append(";");
        }

        numArts++;
        output << "^^" << temp->title << ";" << theText << ";" << peopleAndOrgs << "\n";

        if (numArts % 1000 == 0) {
            system("clear");
            printHeader();
            std::cout << "Relative path to the corpus file: " << fileLocation << "\n\n";
            end = std::chrono::high_resolution_clock::now();
            std::cout << "Wrote " << numArts << " articles in " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds.\n";
        }
    }

    // Writes Nodes
    int numNodes = 0;
    std::vector<std::pair<std::string, std::map<ArticleObject*, int>>> nodes = theTree->getNodesFromTree(theTree->root);
    for (size_t i = 0; i < nodes.size(); i++) {
        output << "--" << nodes[i].first;
        auto theMap = nodes[i].second;
        std::vector<std::pair<ArticleObject*, int>> currArts;
        for (auto it = theMap.begin(); it != theMap.end(); ++it) {
            currArts.push_back(std::make_pair(it->first, it->second));
        }
        for (size_t j = 0; j < currArts.size(); j++) {
            auto it = std::find(theArticles.begin(), theArticles.end(), currArts[j].first);
            size_t index = std::distance(theArticles.begin(), it);
            output << index << "-" << currArts[j].second << ",";
        }
        output << "\n";
        numNodes++;

        if (numNodes % 10000 == 0) {
            system("clear");
            printHeader();
            std::cout << "Relative path to the corpus file: " << fileLocation << "\n\n";
            end = std::chrono::high_resolution_clock::now();
            std::cout << "Wrote " << numNodes << " TreeNodes in " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds.\n";
        }
    }

    output.close();
}

int PersistanceOperator::readTreeFromFile(std::string fileLocation, AVLTree* indexTree, AVLTree* personTree, AVLTree* orgTree) {  // The reverse of writeTreeFromFile(). Reconstructs the tree from the file in memory
    std::ifstream input(fileLocation);
    if (!input.is_open()) {
        std::cerr << "Cannot open file: " << fileLocation << "\n\nPress any key to continue.\n> ";
        std::string temp;
        std::getline(std::cin, temp, '\n');
        return 0;
    }
    std::string line;
    std::vector<ArticleObject*> articleArray;
    int numNodes = 0;
    int numArts = 0;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    while (std::getline(input, line)) {
        // Reads articles
        if (line.rfind("^^", 0) == 0) {
            std::string title;
            std::string text;
            std::vector<std::string> people;
            std::vector<std::string> orgs;
            std::vector<std::string> tokenized = split(line, ';');

            title = tokenized[0].substr(2);

            text = tokenized[1];
            size_t pos = text.find('~', pos + 1);
            while (pos != std::string::npos) {
                text.replace(pos, 1, "\n");
                pos = text.find('~', pos + 1);
            }

            pos = text.find('`');
            while (pos != std::string::npos) {
                text.replace(pos, 1, ";");
                pos = text.find('`', pos + 1);
            }

            people = split(tokenized[2], ',');

            orgs = split(tokenized[3], ',');

            articleArray.push_back(new ArticleObject(title, text, people, orgs));
            numArts++;
            if (numArts % 1000 == 0) {
                system("clear");
                printHeader();
                std::cout << "Relative path to the corpus file: " << fileLocation << "\n\n";
                end = std::chrono::high_resolution_clock::now();
                std::cout << "Read " << numArts << " articles in " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds.\n";
            }
        } else if (line.rfind("--", 0) == 0) {  // Reands Nodes
            std::string ID;
            std::map<ArticleObject*, int> data;
            std::vector<std::string> tokenized = split(line, ';');

            ID = tokenized[0].substr(2);
            std::vector<std::string> mapList = split(tokenized[2], ',');
            for (size_t i = 0; i < mapList.size(); i++) {
                std::vector<std::string> curr = split(mapList[i], '-');
                data[articleArray[std::stoi(curr[0])]] = std::stoi(curr[1]);
            }
            indexTree->AddNodeMap(data, ID);
            numNodes++;
            if (numNodes % 10000 == 0) {
                system("clear");
                printHeader();
                std::cout << "Relative path to the corpus file: " << fileLocation << "\n\n";
                end = std::chrono::high_resolution_clock::now();
                std::cout << "Created " << numNodes << " TreeNodes in " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds.\n";
            }
        } else {
            continue;
        }
    }

    // Update term counrs
    indexTree->numElements = numNodes;
    personTree->numElements = numNodes;
    orgTree->numElements = numNodes;

    return numArts;
}

#endif