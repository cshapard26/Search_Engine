#ifndef DOCUMENTPARSER
#define DOCUMENTPARSER

#include <bits/stdc++.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"
#include "ArticleObject.cpp"
#include "stemmer/Porter2Stemmer.cpp"

class DocumentParser {
   private:
    std::unordered_set<std::string> stopWords;

   public:
    std::vector<std::string> removeStopWords(std::vector<std::string>);
    std::vector<std::string> stemWords(std::vector<std::string>);
    std::vector<std::string> cleanWords(std::vector<std::string>);
    std::vector<std::string> StopStemClean(const std::vector<std::string>*);
    std::string StopStemCleanOne(const std::string);
    ArticleObject* convertDocumentToArticleObject(std::string);
    DocumentParser();
};

#endif