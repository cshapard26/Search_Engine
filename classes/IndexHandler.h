#ifndef INDEXHANDLER
#define INDEXHANDLER

#include <iostream>
#include <string>
#include <vector>

#include "AVLTree.cpp"
#include "DocumentParser.cpp"
#include "PersistanceOperator.cpp"

class IndexHandler {
   private:
    AVLTree* termIndex;
    AVLTree* personIndex;
    AVLTree* orgIndex;
    PersistanceOperator persistance;

   public:
    DocumentParser parser;
    int indexingTime;
    int articleNum;
    int getTotalWords();
    IndexHandler(AVLTree*, AVLTree*, AVLTree*, DocumentParser);
    void AddItemToIndex(std::string);
    void AddDirectoryToIndex(std::string);
    std::map<ArticleObject*, int>* GetRelevantResults(std::string, int);
    void SetDefaultIndexies(AVLTree*, AVLTree*, AVLTree*);
    void saveAVL(std::string);
    void loadAVL(std::string);
};

#endif