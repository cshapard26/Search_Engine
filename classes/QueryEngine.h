#ifndef QUERYENGINE
#define QUERYENGINE

#include <cstdlib>
#include <iostream>
#include <string>

#include "IndexHandler.cpp"

class QueryEngine {
   private:
    IndexHandler handler;
    std::string writeAddress;

   public:
    QueryEngine(IndexHandler);
    void saveAVL(std::string);
    void loadAVL(std::string);
    bool UI();
    void setIndexHandler(IndexHandler);
    bool query(std::string);
    void addArticles(std::string);
    void saveToDisk();
    void loadFromDisk();
};

#endif