#ifndef INDEXHANDLERCPP
#define INDEXHANDLERCPP

#include "IndexHandler.h"

#include <chrono>
#include <filesystem>

IndexHandler::IndexHandler(AVLTree* term = nullptr, AVLTree* person = nullptr, AVLTree* org = nullptr, DocumentParser parse = DocumentParser()) {
    termIndex = term;
    personIndex = person;
    orgIndex = org;
    parser = parse;
    indexingTime = 0;
    articleNum = 0;
    persistance = PersistanceOperator();
}

std::string combine(std::vector<std::string> wordList) {  // Combines words in a list with spaces. The opposite of "split()"
    std::string outputString = "";
    for (size_t i = 0; i < wordList.size(); i++) {
        outputString.append(wordList[i]);
        outputString.append(" ");
    }

    return outputString;
}

void IndexHandler::AddItemToIndex(std::string JSONLocation) {  // Adds an ArticleObject to a specific AVLTree depending on the type (term, person, org)
    ArticleObject* theArticle = parser.convertDocumentToArticleObject(JSONLocation);
    AVLTree* theIndex = termIndex;
    std::vector<std::string> theWords = split(theArticle->articleText, ' ');
    theWords = parser.StopStemClean(&theWords);
    for (size_t i = 0; i < theWords.size(); i++) {
        theIndex->AddNode(theArticle, theWords[i]);
    }
    articleNum++;
    theIndex = personIndex;
    std::vector<std::string> thePeople = theArticle->people;
    for (size_t i = 0; i < thePeople.size(); i++) {
        std::vector<std::string> persons = split(thePeople[i], ' ');
        for (size_t j = 0; j < persons.size(); j++) {
            theIndex->AddNode(theArticle, persons[j]);
        }
    }
    theIndex = orgIndex;
    std::vector<std::string> theOrgs = theArticle->orgs;
    for (size_t i = 0; i < theOrgs.size(); i++) {
        std::vector<std::string> orgs = split(theOrgs[i], ' ');
        for (size_t j = 0; j < orgs.size(); j++) {
            theIndex->AddNode(theArticle, orgs[j]);
        }
    }
}

void IndexHandler::AddDirectoryToIndex(std::string JSONLocation) {  // Recursively loops through every article in a directory and adds the nodes to the desired index. Reports timing.
    std::ifstream input(JSONLocation);
    if (!input.is_open()) {
        std::cerr << "Cannot open file: " << JSONLocation << "\n\n";
        std::cout << "Press any key to continue\n> ";
        std::getline(std::cin, JSONLocation, '\n');
        return;
    }
    input.close();
    auto it = std::filesystem::recursive_directory_iterator(JSONLocation);
    int numAdditions = 0;
    auto start = std::chrono::high_resolution_clock::now();
    auto bigstart = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "\n";

    // loop over all the entries.
    for (const auto& entry : it) {
        if (entry.is_regular_file() && entry.path().extension().string() == ".json") {
            AddItemToIndex(entry.path().string());
            numAdditions++;
            if (numAdditions % 1000 == 0) {
                end = std::chrono::high_resolution_clock::now();
                system("clear");
                printHeader();
                std::cout << "Relative path to the new article's file/directory: " << JSONLocation << "\n\n";
                std::cout << "Read " << numAdditions << " files in " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds. There are currently " << termIndex->numElements << " unique words in the index.\n";
            }
        }
    }
    auto bigend = std::chrono::high_resolution_clock::now();
    indexingTime += std::chrono::duration_cast<std::chrono::microseconds>(bigend - bigstart).count();

    return;
}

std::map<ArticleObject*, int>* IndexHandler::GetRelevantResults(std::string query, int treeType) {  // Calls the FindValue() function of the desired index (term, person, org) and returns the result.
    if (treeType == 0) {
        return termIndex->FindValue(query);
    } else if (treeType == 1) {
        return personIndex->FindValue(query);
    } else if (treeType == 2) {
        return orgIndex->FindValue(query);
    } else {
        std::cerr << "Warning: " << treeType << " is not a valid tree type. Returning null result.\n";
        return nullptr;
    }
}

void IndexHandler::SetDefaultIndexies(AVLTree* term = nullptr, AVLTree* person = nullptr, AVLTree* org = nullptr) {
    termIndex = term;
    personIndex = person;
    orgIndex = org;
}

int IndexHandler::getTotalWords() {
    return termIndex->numElements;
}

void IndexHandler::saveAVL(std::string writeAddress) {  // Easier way to call PersistanceOperator::writeTreeToFile
    persistance.writeTreeToFile(termIndex, writeAddress);
}

void IndexHandler::loadAVL(std::string readAddress) {  // Easier way to call PersistanceOperator::readTreeFromFile. Also updates indexing time
    auto start = std::chrono::high_resolution_clock::now();
    articleNum += persistance.readTreeFromFile(readAddress, termIndex, personIndex, orgIndex);
    auto end = std::chrono::high_resolution_clock::now();
    indexingTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

#endif