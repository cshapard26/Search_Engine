#ifndef AVLTREE
#define AVLTREE

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "ArticleObject.cpp"

class AVLTree {
   private:
    struct TreeNode {
        TreeNode* leftChild;
        TreeNode* rightChild;
        std::map<ArticleObject*, int> data;  // Value
        std::string ID;                      // Key
        int height;

        void deallocateArticleObjects() {
            static std::set<ArticleObject*> deletedPointers;

            for (auto it = data.begin(); it != data.end();) {
                ArticleObject* articlePtr = it->first;
                if (deletedPointers.find(articlePtr) == deletedPointers.end()) {
                    delete articlePtr;
                    deletedPointers.insert(articlePtr);
                    it = data.erase(it);
                } else {
                    it++;
                }
            }
        }

        TreeNode() {
            leftChild = nullptr;
            rightChild = nullptr;
            data = {};
            ID = "";
            height = 0;
        }
        ~TreeNode() {
            deallocateArticleObjects();
        }

        TreeNode(ArticleObject* d = new ArticleObject(), std::string id = "", TreeNode* left = nullptr, TreeNode* right = nullptr, int heit = 0) {
            leftChild = left;
            rightChild = right;
            data[d] = 1;
            ID = id;
            height = heit;
        }
        TreeNode(std::map<ArticleObject*, int>* mapp = {}, std::string id = "", TreeNode* left = nullptr, TreeNode* right = nullptr, int heit = 0) {
            leftChild = left;
            rightChild = right;
            data.insert(mapp->begin(), mapp->end());
            ID = id;
            height = heit;
        }
    };
    bool duplicateVal;
    std::map<ArticleObject*, int> returnEmpty = {{new ArticleObject(), 0}};

   public:
    TreeNode* root;
    int numElements;

    TreeNode* BalanceTree(TreeNode*);
    TreeNode* Insert(TreeNode*, ArticleObject*, std::string);
    TreeNode* Remove(TreeNode*, std::string);
    void AddNode(ArticleObject*, std::string);
    TreeNode* RightRotate(TreeNode*);
    TreeNode* LeftRotate(TreeNode*);
    void DeleteNode(std::string);

    std::map<ArticleObject*, int>* FindValue(std::string);
    void clearTree(TreeNode*);
    void printTree(TreeNode*, int);
    std::set<ArticleObject*> getArticlesFromTree(TreeNode*);
    std::vector<std::pair<std::string, std::map<ArticleObject*, int>>> getNodesFromTree(TreeNode*);
    TreeNode* InsertToMap(TreeNode*, std::map<ArticleObject*, int>, std::string);
    void AddNodeMap(std::map<ArticleObject*, int>, std::string);

    AVLTree();
    ~AVLTree();
};

#endif