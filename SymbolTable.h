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

struct Ds {
    Ds() : head(nullptr), size(0) {}
    Ds(Node* head, int size) : head(head), size(size) {}
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

struct DDs {
    DDs() : head(nullptr) {}
    DDs(DNode* head) : head(head) {}
    DNode* head;
};

class SymbolTable {
public:
    SymbolTable() {}
    void run(string filename);
    void deleteDs(Ds& ds);
    void deleteDDs(DDs& dds);
    void checkInstruction(Ds& ds, string S);
    void checkINSERT(Ds& ds, string S, int line, int count_line);
    string checkASSIGN(Ds& ds, string S);
    void checkBEGIN(Ds& ds, string S);
    void checkEND(Ds& ds, string S);
    void deleteBLOCK(Ds& ds);
    string checkLOOKUP(Ds& ds, string S);
    void checkPRINT(Ds& ds, string S, int line, int count_line);
    void checkRPRINT(Ds& ds, string S, int line, int count_line);
};


#endif