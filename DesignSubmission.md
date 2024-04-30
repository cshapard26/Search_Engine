# Coding Assignment 4: Search Engine (Design)
## Team: 404 Team Not Found

1. A first UML diagram. [3]

Made with Mermaid.js. View at https://mermaid.live if it doesn't automatically render with your text editor.
```mermaid
---
title: Search Engine
---
classDiagram
    class ResultItem{
        +string articleName
        +string term
        +string person
        +string org
        +int relevancy
        +void clearInfo()
    }
    class TreeNode {
        +TreeNode* leftChild
        +TreeNode* rightChild
        +ResultItem data //Value
        +string ID //Key
        +void clearInfo()
    }
    class AVLTree{
        -int treeDepth
        -int treeBreadth
        -TreeNode* root
        +void BalanceTree()
        +void AddNode(ResultItem)
        +ResultItem findValue(string)
        +void clearTree()
    }
    class TermIndex{
        -AVLTree termMap
        +void addItem(ResultItem)
        +ResultItem getItem(string)
    }
    class PersonIndex{
        -AVLTree personMap
        +void addItem(ResultItem)
        +ResultItem getItem(string)
    }
    class OrgIndex{
        -AVLTree orgMap
        +void addItem(ResultItem)
        +ResultItem getItem(string)
    }
    class IndexHandler{
        -TermIndex termIndex
        -PersonIndex personIndex
        -OrgIndex orgIndex
        -PersistanceOperator persistance
        +void addItemToAllIndexies(ResultItem, TermIndex, PersonIndex, OrgIndex)
        +ResultItem[] getRelevantResults(string, TermIndex, PersonIndex, OrgIndex)
        +void setDefaultIndexies(ResultItem, TermIndex, PersonIndex, OrgIndex)
        +void setPersistanceOperator(PersistanceOperator)
    }
    class DocumentParser{
        -string[] stopWords
        +JSON removeStopWords(JSON)
        +JSON stemWords(JSON)
        +ResultItem setDefaultRevevancy(ResultItem)
        +ResultItem convertDocumentToResultItem(JSON)
    }

    class QueryEngine{
        -bool writeAccess
        -IndexHandler handler
        -void saveAVL()
        -void loadAVL()
        +void displayPromptScreen()
        +ResultItem findPrompt(string)
        +void setIndexHandler(IndexHandler)
    }

    class PersistanceOperator{
        -bool newChanges
        +bool writeTreeToMemory(TermIndex)
        +TermIndex readTermTreeFromMemory()
        +PersonIndex readPersonTreeFromMemory()
        +OrgIndex readOrgTreeFromMemory()
    }


    ResultItem ..> TreeNode
    TreeNode ..> AVLTree
    AVLTree ..> TermIndex
    AVLTree ..> PersonIndex
    AVLTree ..> OrgIndex
    TermIndex <--> IndexHandler
    PersonIndex <--> IndexHandler
    OrgIndex <--> IndexHandler
    DocumentParser <--> IndexHandler
    IndexHandler <--> QueryEngine
    PersistanceOperator <--> IndexHandler
    DocumentParser --> ResultItem

```

2. A description of what each class does and what data structures it uses. [3]

| Class | Description | Data Structures/Attributes |
|---|---|---|
| ResultItem | A class used to store lot's of information about an article.<br>The lowest packet of information in my system| None |
| TreeNode | A single node in the AVL tree to be used in the AVLTree class.<br>Has key and value attributes. | Map |
| AVLTree | A basic AVL tree with balancing functionalities. | Tree |
| TermIndex/<br>PersonIndex/<br>OrgIndex | The inverted file index for each type of query.<br>Simplifies the AVLTree class. | Tree |
| IndexHandler | Requests and pushes information to the specified Indexies.<br>Can return an array of the keys sorted by relevancy | Array |
| DocumentParser | Cleans and processes data for input into the IndexHandler | RapidJSON |
| QueryEngine | The main user control unit for the system.<br>Allows for simplified function calls and control. | None |
| PersistanceOperator | Loads and stores a standardized format<br> of the AVL tree to and from memory | File Handling |

3. A short description of how the classes interact with each other to create an index and perform a query. [2]

- In this system, the user starts off by creating an index with a directory of data. This goes through the QueryEngine to the IndexHandler, which puts each document through the document parser, which cleans the data and turns it into ReturnItems. These ReturnItems are then sorted into their appropriate AVLTree Index (Item, Person, or Org) and assigned a relevancy. To perform a request/query, the user inputs a request through the QueryEngine, which asks the IndexHandler for the specified data. The IndexHander goes through the desired Indexies and finds all matching results, puts them in an array sorted by relevancy, which is sent back to be displayed by the user. The QueryEngine can also submit a request for the AVL tree to be stored/loaded from a standardized format using the PersistanceOperator, but only if the user has writeAccess to the data.

4. A timeline for implementing the different pieces with the projected completion date for each component. [2]

```mermaid
timeline
    title Search Engine Timeline—404 Team Not Found
    section Design
        April 3: Class Diagram
        April 5: Class Description and timeline
        April 6: Submit Design Doc
    section Map
        April 10: Finish ResultItem, DocumentParser, AVLTree, and TreeNode classes
        April 12: AVLTree tests and Submit Map Assignment 
        April 16: Finish Indexies, Index Handler, and Persistance Operator classes
        April 20: Finish Query Engine
        April 22: Optimize System
    section Submission
        April 24: Final Testing/Documentation
        April 25: Submit Finalized Project

```
