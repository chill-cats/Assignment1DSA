#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

using namespace std;

class SymbolTable {
public:
    SymbolTable() {}
    void run(string filename);

    struct identifier_name {
        identifier_name(): ID(), type(), value(""), level(0) {}
        identifier_name(string ID, string type, string value, int level): ID(ID), type(type), value(value), level(level) {}
        string ID;
        string type;
        string value;
        int level;
    };

    struct identifier_node {
        identifier_node(): data(identifier_name()), next(nullptr) {}
        identifier_node(identifier_name data, identifier_node* next): data(data), next(next) {}

        identifier_name data;
        identifier_node* next;
        identifier_node* prev;
    };

    struct DLinkedList{
        DLinkedList(): head(nullptr), tail(nullptr), size(0) {}
        DLinkedList(identifier_node* head, identifier_node* tail, int size): head(head), tail(tail), size(size) {}

        identifier_node* head;
        identifier_node* tail;
        int size;
    };

    int count_line(string filename);

    int check_tok(string s);

    bool check_valid_name(string s);

    bool check_valid(string s);

    bool insert_list(DLinkedList &list, identifier_name newID);

    string type_of_value(string value);

    string assign(DLinkedList &list, string ID, string value);

    void end(DLinkedList &list, int level);

    int count_ID(DLinkedList list, string ID);

    void print(DLinkedList list);

    void rprint(DLinkedList list);

    int lookup(DLinkedList list, string ID);

    DLinkedList copy(DLinkedList);

    void delete_same_ID(DLinkedList &list);
};

#endif