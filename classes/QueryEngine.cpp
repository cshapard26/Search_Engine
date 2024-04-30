#ifndef QUERYENGINECPP
#define QUERYENGINECPP

#include "QueryEngine.h"

#include <bits/stdc++.h>

#include <algorithm>
#include <cmath>
#include <list>

QueryEngine::QueryEngine(IndexHandler theHandler = IndexHandler()) {
    handler = theHandler;
    writeAddress = "supersearch.corpus";  // The default filename if one is not provided
}

bool compare(const std::pair<ArticleObject*, int>& a, const std::pair<ArticleObject*, int>& b) {  // this function was sourced from rashedcs (https://stackoverflow.com/questions/9370945/finding-the-max-value-in-a-map)
    return a.second < b.second;
}

void QueryEngine::saveToDisk() {  // Calls IndexHandler::saveAVL, but with the default filename
    handler.saveAVL(writeAddress);
}

void QueryEngine::loadFromDisk() {  // Calls IndexHandler::loadAVL, but with the default filename
    handler.loadAVL(writeAddress);
}

void QueryEngine::addArticles(std::string filepath) {  // Easier call to IndexHandler::AddDirectoryToIndex
    handler.AddDirectoryToIndex(filepath);
}

bool QueryEngine::query(std::string queries) {  // performs a query for a specific term
    std::vector<std::string> queryTerms = split(queries, ' ');
    bool dontShow = false;
    std::vector<ArticleObject*> dontShowList;  // For terms with a "-" prefix
    int type = 0;
    std::map<ArticleObject*, int> queryReturn;

    auto start = std::chrono::high_resolution_clock::now();
    if (queries == "") return true;

    // Cleans each input and performs a query from its respective index (depending on PERSON, ORG, -, or no prefix). Calculates the relevancy for each article depending on its base rate and term completeness
    for (size_t i = 0; i < queryTerms.size(); i++) {
        if (queryTerms[i].rfind("PERSON:") == 0) {
            type = 1;
            queryTerms[i] = queryTerms[i].substr(7, queryTerms[i].size() - 7);
        } else if (queryTerms[i].rfind("ORG:") == 0) {
            type = 2;
            queryTerms[i] = queryTerms[i].substr(4, queryTerms[i].size() - 4);
        } else if (queryTerms[i].rfind("-") == 0) {
            type = 0;
            dontShow = true;
            queryTerms[i] = queryTerms[i].substr(1, queryTerms[i].size() - 1);
            queryTerms[i] = handler.parser.StopStemCleanOne(queryTerms[i]);
        } else {
            type = 0;
            queryTerms[i] = handler.parser.StopStemCleanOne(queryTerms[i]);
        }
        std::map<ArticleObject*, int>* tempMap = handler.GetRelevantResults(queryTerms[i], type);
        for (auto i = tempMap->begin(); i != tempMap->end(); i++) {
            if (!dontShow) {
                if (queryReturn.count(i->first)) {
                    queryReturn[i->first] += std::pow(i->second, 1.0 / 3.0);  // My algorithm for smart weighting. Cube roots the standard relevancy and adds it to the query, which means more weight is put on articles that have all the desired terms, but it still accurately ranks those that don't
                } else {
                    queryReturn[i->first] = std::pow(i->second, 1.0 / 3.0);
                }
            } else {
                dontShowList.push_back(i->first);
            }
        }
        dontShow = false;
    }

    // Removes articles that have terms with a - from the return list
    if (dontShowList.size() > 0) {
        for (size_t i = 0; i != dontShowList.size(); i++) {
            auto theItem = dontShowList[i];
            if (queryReturn.count(theItem) > 0) {
                queryReturn.erase(theItem);
            }
        }
    }

    int numResults = queryReturn.size();
    std::vector<ArticleObject*> orderedReturnObject;
    auto iterator = queryReturn.begin();

    // Grabs only the articles with the top 15 relevancies. Also prevents duplicate titles from appearing. If there are less than 15 relevant results, add them all in order.
    for (auto i = 0; i < 15; i++) {
        if (queryReturn.begin() == queryReturn.end()) {
            if (queryReturn.begin()->first != nullptr) {
                orderedReturnObject.push_back(queryReturn.begin()->first);
            }
            break;
        }
        auto max = std::max_element(queryReturn.begin(), queryReturn.end(), compare)->first;
        bool found = false;
        for (size_t j = 0; j < orderedReturnObject.size(); j++) {
            if (max->title == orderedReturnObject[j]->title) found = true;
        }
        if (!found) {
            orderedReturnObject.push_back(max);
        } else {
            found = false;
            i--;
        }
        queryReturn.erase(max);
        iterator++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    if (orderedReturnObject.size() > 0)
        if (orderedReturnObject[0]->title == "") numResults--;  // Fixes edge case where an empty term is counted as a result

speakCode:  // This line creates a branchpoint so that the user can return and view other articles without having to query again.
    system("clear");
    printHeader();

    // Displays and numbers the results
    std::cout << numResults << ((numResults == 1) ? (" article found for \"" + queries + "\". Search took ") : (" articles found for \"" + queries + "\". Search took ")) << (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < 1000 ? std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() : std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) << ((std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < 1000) ? " microseconds\n" : " milliseconds\n");
    std::cout << "Search Results: \n";
    std::cout << "    0. Return to menu\n";
    if (orderedReturnObject.size() > 0) {
        for (size_t i = 0; i < orderedReturnObject.size(); i++) {
            if (orderedReturnObject[i]->title != "") std::cout << "    " << i + 1 << ". " << orderedReturnObject[i]->title << "\n";
        }
    }

    std::string userResponse = "";
    std::cout << "\n";
    std::vector<std::string> responses = {"0"};

    // Only allows the user to input an article number that exists
    if (numResults > 0) {
        for (size_t i = 1; i < orderedReturnObject.size() + 1; i++) {
            responses.push_back(std::to_string(i));
        }
    }

    // Prompts user to choose an article and displays the text of the article they select
    while ((std::find(responses.begin(), responses.end(), userResponse) == responses.end())) {
        std::cout << "> ";
        std::getline(std::cin, userResponse, '\n');
    }
    if (userResponse == "0" || userResponse == "") {
        system("clear");
        return true;
    } else {
        int choice = std::stoi(userResponse);
        system("clear");
        std::cout << orderedReturnObject[choice - 1]->articleText << "\n\nPress any button to return.\n";
        std::cout << "> ";
        std::getline(std::cin, userResponse, '\n');
        userResponse = "";
        goto speakCode;
    }
}

bool QueryEngine::UI() {  // Displays a simple graphical user interface and calls the respective functions based on user input. Also displays statistics
    system("clear");
    printHeader();
    std::cout << "Please select an option: \n";
    std::cout << "1. Submit a query\n";
    std::cout << "2. Add article(s) to corpus\n";
    std::cout << "3. Save corpus to file\n";
    std::cout << "4. Load corpus from file\n";
    std::cout << "5. Display Statistics\n";
    std::cout << "6. Close the Program\n";

    std::string userInput = "";
    std::cout << "> ";
    std::getline(std::cin, userInput, '\n');
    system("clear");
    printHeader();

    if (userInput == "1") {
        std::string queries = "";
        std::cout << "Submit your query terms: ";
        std::getline(std::cin, queries, '\n');

        return query(queries);

    } else if (userInput == "2") {
        system("clear");
        printHeader();

        std::string filepath;
        std::cout << "Relative path to the new article's file/directory: ";
        std::getline(std::cin, filepath, '\n');
        addArticles(filepath);

    } else if (userInput == "3") {
        std::string filename;
        std::cout << "Name your new corpus file: ";
        std::getline(std::cin, filename, '\n');
        handler.saveAVL(filename);
    } else if (userInput == "4") {
        std::string filepath;
        std::cout << "Relative path to the corpus file: ";
        std::getline(std::cin, filepath, '\n');
        handler.loadAVL(filepath);

    } else if (userInput == "5") {
        std::cout << "Total Indexing Time: " << (handler.indexingTime > 1000000 ? std::to_string(handler.indexingTime / 1000000) + " seconds" : (handler.indexingTime > 1000 ? std::to_string(handler.indexingTime / 1000) + " milliseconds" : std::to_string(handler.indexingTime) + " microseconds")) << ".\n";
        std::cout << "Total Articles: " << handler.articleNum << "\n";
        std::cout << "Total Words Indexed: " << handler.getTotalWords() << "\n\n";

        std::string userResponse;
        std::cout << "Press any button to return.\n";
        std::cout << "> ";
        std::getline(std::cin, userResponse, '\n');
    } else if (userInput == "6") {
        system("clear");
        return false;
    }

    system("clear");
    return true;
}

void QueryEngine::setIndexHandler(IndexHandler theHandler) {
    handler = theHandler;
}

#endif