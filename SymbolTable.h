#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

struct Node {
    Node() : data(""), value(""), level(0), next(nullptr) {}
    Node(string data, string value, int level, Node* next) : data(data), value(value), level(level), next(next) {}
    string data, value;
    int level;
    Node* next;
};

struct List {
    List() : head(nullptr), size(0) {}
    List(Node* head, int size) : head(head), size(size) {}
    Node* head;
    int size;
};

struct DNode {
    DNode() : data(""), value(""), level(0), next(nullptr), prev(nullptr) {}
    DNode(string data, string value, int level, DNode* next, DNode* prev) : data(data), value(value), level(level), next(next), prev(prev) {}
    string data, value;
    int level;
    DNode* next, * prev;
};

struct DList {
    DList() : head(nullptr) {}
    DList(DNode* head) : head(head) {}
    DNode* head;
};

class SymbolTable {
public:
    SymbolTable() {}
    void run(string filename);
    void deleteList(List& list);
    void deleteDList(DList& dlist);
    void checkInstruction(List& list, string S);
    void checkINSERT(List& list, string S, int line, int count_line);
    string checkASSIGN(List& list, string S);
    void checkBEGIN(List& list, string S);
    void checkEND(List& list, string S);
    void deleteBLOCK(List& list);
    string checkLOOKUP(List& list, string S);
    void checkPRINT(List& list, string S, int line, int count_line);
    void checkRPRINT(List& list, string S, int line, int count_line);
};


#endif