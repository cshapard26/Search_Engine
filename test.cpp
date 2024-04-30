#define CATCH_CONFIG_MAIN
#include "classes/AVLTree.cpp"
#include "classes/IndexHandler.cpp"
#include "classes/PersistanceOperator.cpp"
#include "classes/catch2/catch.hpp"

TEST_CASE("AVLTree Tests") {
    AVLTree tree;

    // Test insertion and Find dunction
    SECTION("Insertion") {
        ArticleObject* firstNode = new ArticleObject();
        tree.AddNode(firstNode, "1");
        tree.AddNode(firstNode, "2");

        REQUIRE(tree.FindValue("1")->size() == 1);
        REQUIRE(tree.FindValue("2")->size() == 1);
    }

    // Test balance
    SECTION("Balance") {
        ArticleObject* firstItem = new ArticleObject();

        // Adding values in random order to show it doesn't require sequential insertions
        tree.AddNode(firstItem, "a");
        tree.AddNode(firstItem, "c");
        tree.AddNode(firstItem, "v");
        tree.AddNode(firstItem, "w");
        tree.AddNode(firstItem, "b");
        tree.AddNode(firstItem, "d");
        tree.AddNode(firstItem, "f");
        tree.AddNode(firstItem, "e");
        tree.AddNode(firstItem, "g");
        tree.AddNode(firstItem, "i");
        tree.AddNode(firstItem, "h");
        tree.AddNode(firstItem, "o");
        tree.AddNode(firstItem, "n");
        tree.AddNode(firstItem, "t");
        tree.AddNode(firstItem, "s");
        tree.AddNode(firstItem, "u");
        tree.AddNode(firstItem, "x");
        tree.AddNode(firstItem, "y");
        tree.AddNode(firstItem, "q");
        tree.AddNode(firstItem, "j");
        tree.AddNode(firstItem, "k");
        tree.AddNode(firstItem, "l");
        tree.AddNode(firstItem, "m");
        tree.AddNode(firstItem, "p");
        tree.AddNode(firstItem, "r");

        // Tests
        // Check if tree is properly balanced
        REQUIRE(tree.root->height >= std::log(tree.numElements) / std::log(2));
    }

    // Test deletion
    SECTION("Deletion") {
        ArticleObject* firstNode = new ArticleObject();
        tree.AddNode(firstNode, "1");
        tree.AddNode(firstNode, "2");
        tree.DeleteNode("1");

        REQUIRE(tree.FindValue("1")->begin()->first != tree.FindValue("2")->begin()->first);
        REQUIRE(tree.root->height >= std::log(tree.numElements) / std::log(2));
    }

    // Test tree traversal
    SECTION("Tree Traversal") {
        ArticleObject* firstNode = new ArticleObject();
        ArticleObject* secondNode = new ArticleObject();
        ArticleObject* thirdNode = new ArticleObject();
        ArticleObject* fourthNode = new ArticleObject();

        tree.AddNode(firstNode, "1");
        tree.AddNode(secondNode, "2");
        tree.AddNode(thirdNode, "3");
        tree.AddNode(fourthNode, "4");

        std::set<ArticleObject*> articles = tree.getArticlesFromTree(tree.root);
        REQUIRE(articles.size() == 4);
    }

    // Test empty tree
    SECTION("Empty Tree") {
        REQUIRE(tree.getArticlesFromTree(tree.root).empty());
    }

    SECTION("Clear Tree") {
        tree.clearTree(tree.root);
        REQUIRE(tree.root == nullptr);
    }
}

TEST_CASE("PersistanceOperator Tests") {
    SECTION("Write Tree To File") {
        AVLTree tree;

        PersistanceOperator persistanceOperator;
        persistanceOperator.writeTreeToFile(&tree, "test_output.txt");

        // Check if the file was cretaed
        std::ifstream file("test_output.txt");
        REQUIRE(file.good());
        file.close();
    }

    SECTION("Read Tree From File") {
        AVLTree indexTree, personTree, orgTree;
        PersistanceOperator persistanceOperator;

        int numArticles = persistanceOperator.readTreeFromFile("test.corpus", &indexTree, &personTree, &orgTree);

        // Check number of articles
        REQUIRE(numArticles == 6);
    }

    // Additional tests for other functions in PersistanceOperator class
}

TEST_CASE("Memory Leak Tests") {
    SECTION("Index") {
        std::string command = "valgrind --leak-check=full ./supersearch index sample_data";
        int returnCode = system(command.c_str());
        REQUIRE(returnCode == 0);
    }
    SECTION("Query") {
        std::string command = "valgrind --leak-check=full ./supersearch query \"German\"";
        int returnCode = system(command.c_str());
        REQUIRE(returnCode == 0);
    }
}

TEST_CASE("IndexHandler Tests") {
    // Create an instance of IndexHandler
    AVLTree* termIndex = new AVLTree();
    AVLTree* personIndex = new AVLTree();
    AVLTree* orgIndex = new AVLTree();
    IndexHandler indexHandler(termIndex, personIndex, orgIndex);

    SECTION("Add Directory") {
        indexHandler.AddDirectoryToIndex("sample_data/coll_1");
        REQUIRE(termIndex->numElements == 186);
        REQUIRE(personIndex->numElements == 11);
        REQUIRE(orgIndex->numElements == 15);
    }

    SECTION("Check Results") {
        indexHandler.AddDirectoryToIndex("sample_data/coll_1");
        std::string query = "german";
        std::map<ArticleObject*, int>* results = indexHandler.GetRelevantResults(query, 0);
        REQUIRE(results->size() == 1);
        REQUIRE(results->begin()->first->title == "German firms doing business in UK gloomy about Brexit - survey");
    }
}
