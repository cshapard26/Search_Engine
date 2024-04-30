#ifndef PERSISTANCEOPERATOR
#define PERSISTANCEOPERATOR

#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "AVLTree.cpp"

class PersistanceOperator {
   public:
    PersistanceOperator();
    void writeTreeToFile(AVLTree*, std::string);
    int readTreeFromFile(std::string, AVLTree*, AVLTree*, AVLTree*);
};

#endif