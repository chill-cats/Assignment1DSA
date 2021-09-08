#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable {
public:
  SymbolTable() {}
  void run(string filename);

  struct Node {
    Node() : data(""), value(""), level(0), next(nullptr) {}

    Node(string data, string value, int level, Node *next)
        : data(data), value(value), level(level), next(next) {}

    string data, value;
    int level;
    Node *next;
  };

  struct List {
    Node *head;
    int size;

    List() : head(nullptr), size(0) {}
    List(Node *head, int size) : head(head), size(size) {}
    ~List();
  };

  struct DNode {
    DNode() : data(""), value(""), level(0), next(nullptr), prev(nullptr) {}
    DNode(string data, string value, int level, DNode *next, DNode *prev)
        : data(data), value(value), level(level), next(next), prev(prev) {}
    string data, value;
    int level;
    DNode *next, *prev;
  };
  bool checkInstruction(string);
  string checkINSERT(List &list, string S);
  string checkASSIGN(List &list, string S);
  void checkBEGIN(List &list, string S);
  void checkEND(List &list, string S);
  void deleteBLOCK(List &list);
  string checkLOOKUP(List &list, string S);
  void checkPRINT(List &list, string S, int line, int count_line);
  void checkRPRINT(List &list, string S, int line, int count_line);
};
#endif
