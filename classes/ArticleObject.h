#ifndef ARTICLEOBJECT
#define ARTICLEOBJECT

#include <string>
#include <vector>

class ArticleObject {
   public:
    std::string title;
    std::string articleText;
    std::vector<std::string> people;
    std::vector<std::string> orgs;
    ArticleObject(std::string, std::string, std::vector<std::string>, std::vector<std::string>);
};

#endif