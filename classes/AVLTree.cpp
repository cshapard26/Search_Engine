#ifndef AVLTREECPP
#define AVLTREECPP

#include "AVLTree.h"

AVLTree::AVLTree() {
    root = nullptr;
    numElements = 0;
    duplicateVal = false;
}

AVLTree::~AVLTree() {
    clearTree(root);
    root = nullptr;  // the "tag" for an empty tree
    delete returnEmpty.begin()->first;
}

AVLTree::TreeNode* AVLTree::RightRotate(TreeNode* theNode) {  // Right Rotates two nodes in the AVLTRee
    TreeNode* temp = theNode->leftChild;
    TreeNode* temp2 = temp ? temp->rightChild : nullptr;

    temp->rightChild = theNode;
    theNode->leftChild = temp2;

    theNode->height = (!theNode->rightChild && !theNode->leftChild) ? 0 : 1 + std::max(theNode->rightChild ? theNode->rightChild->height : 0, theNode->leftChild ? theNode->leftChild->height : 0);
    temp->height = (!temp->rightChild && !temp->leftChild) ? 0 : 1 + std::max(temp->rightChild ? temp->rightChild->height : 0, temp->leftChild ? temp->leftChild->height : 0);

    return temp;
}

AVLTree::TreeNode* AVLTree::LeftRotate(TreeNode* theNode) {  // Left Rotates two nodes in the AVL Tree
    TreeNode* temp = theNode->rightChild;
    TreeNode* temp2 = temp ? temp->leftChild : nullptr;

    temp->leftChild = theNode;
    theNode->rightChild = temp2;

    theNode->height = (!theNode->rightChild && !theNode->leftChild) ? 0 : 1 + std::max(theNode->rightChild ? theNode->rightChild->height : 0, theNode->leftChild ? theNode->leftChild->height : 0);
    temp->height = (!temp->rightChild && !temp->leftChild) ? 0 : 1 + std::max(temp->rightChild ? temp->rightChild->height : 0, temp->leftChild ? temp->leftChild->height : 0);

    return temp;
}

AVLTree::TreeNode* AVLTree::BalanceTree(TreeNode* theNode) {  // Checks if a node is left or right leaning, then rebalances it with rotations
    bool left = theNode->leftChild;
    bool right = theNode->rightChild;
    bool rightLeaning = false;
    bool leftLeaning = false;

    int balanceFactor = (left ? theNode->leftChild->height : 0) - (right ? theNode->rightChild->height : 0);
    if (balanceFactor >= 1) {
        leftLeaning = true;
    } else if (balanceFactor <= -1) {
        rightLeaning = true;
    }
    TreeNode* returnNode = theNode;

    // Right Rotate
    if (leftLeaning) {
        // Left Rotate first?
        if (left) {
            if (theNode->leftChild->rightChild) {
                theNode->leftChild = LeftRotate(theNode->leftChild);
            }
        }
        returnNode = RightRotate(theNode);

        // Left Rotate
    } else if (rightLeaning) {
        // Right rotate first?
        if (right) {
            if (theNode->rightChild->leftChild) {
                theNode->rightChild = RightRotate(theNode->rightChild);
            }
        }
        returnNode = LeftRotate(theNode);
    }
    return returnNode;
}

AVLTree::TreeNode* AVLTree::Insert(TreeNode* current, ArticleObject* theItem, std::string theID) {  // Recursively inserts a node based on the value, then rebalances the tree as it exits the recursion
    if (current == nullptr) {
        numElements++;
        return new TreeNode(theItem, theID);
    } else if (theID < current->ID) {
        current->leftChild = Insert(current->leftChild, theItem, theID);
    } else if (theID > current->ID) {
        current->rightChild = Insert(current->rightChild, theItem, theID);

    } else {
        current->data[theItem]++;
        return current;
    }
    current = BalanceTree(current);
    current->height = std::max(current->leftChild ? current->leftChild->height + 1 : 1, current->rightChild ? current->rightChild->height + 1 : 1);
    return current;
}

void AVLTree::AddNode(ArticleObject* theItem, std::string theID) {  // A simpler way to call Insert
    root = Insert(root, theItem, theID);
}

void AVLTree::AddNodeMap(std::map<ArticleObject*, int> theMap, std::string theID) {
    root = InsertToMap(root, theMap, theID);
    numElements = theMap.size();
}

AVLTree::TreeNode* AVLTree::InsertToMap(TreeNode* current, std::map<ArticleObject*, int> theMap, std::string theID) {  // Similar to insert, but it allows inserting a term map instead of just a signle term
    if (current == nullptr) {
        return new TreeNode(&theMap, theID);
    } else if (theID < current->ID) {
        current->leftChild = InsertToMap(current->leftChild, theMap, theID);
    } else if (theID > current->ID) {
        current->rightChild = InsertToMap(current->rightChild, theMap, theID);
    }
    current = BalanceTree(current);
    current->height = std::max(current->leftChild ? current->leftChild->height + 1 : 1, current->rightChild ? current->rightChild->height + 1 : 1);
    return current;
}

void AVLTree::DeleteNode(std::string theID) {  // A simpler way to call Remove()
    root = Remove(root, theID);
    if (!duplicateVal) numElements--;
    duplicateVal = false;
}

AVLTree::TreeNode* AVLTree::Remove(TreeNode* current, std::string theID) {  // Recursively finds a node in the tree, deletes it, then rebalances as it exits recursion
    if (current == nullptr) {
        std::cout << "Attempting to delete unknown ID, " << theID << ", from the tree. Ignoring.\n";
        duplicateVal = true;
        return nullptr;
    }

    if (theID < current->ID) {
        current->leftChild = Remove(current->leftChild, theID);
    } else if (theID > current->ID) {
        current->rightChild = Remove(current->rightChild, theID);
    } else {
        if (current->leftChild == nullptr) {
            TreeNode* temp = current->rightChild;
            delete current;
            return temp;
        } else if (current->rightChild == nullptr) {
            TreeNode* temp = current->leftChild;
            delete current;
            return temp;
        } else {
            TreeNode* temp = current->rightChild;
            while (temp->leftChild != nullptr) {
                temp = temp->leftChild;  // Find smallest node to teh right of current
            }
            TreeNode* temp2 = new TreeNode(&(temp->data), temp->ID, current->leftChild, current->rightChild, current->height);
            delete current;
            if (root == current) {
                root = temp2;
            }
            current = temp2;
            current->rightChild = Remove(current->rightChild, temp->ID);
        }
    }

    current = BalanceTree(current);
    current->height = std::max(current->leftChild ? current->leftChild->height + 1 : 1, current->rightChild ? current->rightChild->height + 1 : 1);

    return current;
}

std::map<ArticleObject*, int>* AVLTree::FindValue(std::string theID) {  // Searches the tree for a value and returns the ArticleObject data
    TreeNode* current = root;

    while (current != nullptr) {
        if (theID > current->ID) {
            current = current->rightChild;
        } else if (theID < current->ID) {
            current = current->leftChild;
        } else {
            return &(current->data);
        }
    }
    return &returnEmpty;
}

void AVLTree::clearTree(TreeNode* current) {  // Clears the tree
    if (current != nullptr) {
        TreeNode* leftChild = current->leftChild;
        TreeNode* rightChild = current->rightChild;

        if (leftChild) clearTree(leftChild);
        if (rightChild) clearTree(rightChild);

        delete current;
        numElements--;
    }
}

void AVLTree::printTree(TreeNode* start, int tabs) {  // A pretty way to print the tree. Only readable for smaller trees
    if (start != nullptr) {
        printTree(start->rightChild, tabs + 1);
        for (int i = 0; i < tabs; i++) {
            std::cout << "\t";
        }
        std::cout << start->ID << "\n";
        printTree(start->leftChild, tabs + 1);
    }
}

std::set<ArticleObject*> AVLTree::getArticlesFromTree(TreeNode* theNode) {  // Grabs all articles present in the tree without duplicates
    std::set<ArticleObject*> theSet;
    if (theNode != nullptr) {
        std::set<ArticleObject*> temp;
        temp = getArticlesFromTree(theNode->leftChild);
        theSet.insert(temp.begin(), temp.end());
        for (auto it = theNode->data.begin(); it != theNode->data.end(); it++) {
            theSet.insert(it->first);
        }
        temp = getArticlesFromTree(theNode->rightChild);
        theSet.insert(temp.begin(), temp.end());
    }
    return theSet;
}

std::vector<std::pair<std::string, std::map<ArticleObject*, int>>> AVLTree::getNodesFromTree(TreeNode* theNode) {  // Returns a list of all the nodes/terms and their associated articles/relevancies
    std::vector<std::pair<std::string, std::map<ArticleObject*, int>>> theList;
    if (theNode != nullptr) {
        auto temp = getNodesFromTree(theNode->leftChild);
        theList.insert(theList.end(), temp.begin(), temp.end());

        std::string temp2;
        temp2.append(theNode->ID);
        temp2.append(";");
        temp2.append(std::to_string(theNode->height));
        temp2.append(";");

        std::map<ArticleObject*, int> temp3 = theNode->data;

        theList.push_back(std::make_pair(temp2, temp3));

        temp = getNodesFromTree(theNode->rightChild);
        theList.insert(theList.end(), temp.begin(), temp.end());
    }
    return theList;
}

#endif