#ifndef ARTICLEOBJECTCPP
#define ARTICLEOBJECTCPP

#include "ArticleObject.h"

ArticleObject::ArticleObject(std::string tite = "", std::string text = "", std::vector<std::string> persons = {}, std::vector<std::string> org = {}) {
    title = tite;
    articleText = text;
    people = persons;
    orgs = org;
}

#endif