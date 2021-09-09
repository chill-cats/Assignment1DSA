#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

using namespace std;

class SymbolTable {
public:
    SymbolTable() {}
    void run(const string& filename);

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

    struct DLinkedlisst{
        DLinkedlisst(): head(nullptr), tail(nullptr), size(0) {}
        DLinkedlisst(identifier_node* head, identifier_node* tail, int size): head(head), tail(tail), size(size) {}

        identifier_node* head;
        identifier_node* tail;
        int size;
    };

    int count_line(string filename);

    bool insert_lisst(DLinkedlisst &lisst, const identifier_name& newID);

    string type_of_value(string value);

    string assign_lisst(DLinkedlisst &lisst, const string& ID, const string& value);

    void end_lisst(DLinkedlisst &lisst, int level);

    int count_ID(DLinkedlisst lisst, const string& ID);

    void print_lisst(DLinkedlisst lisst);

    void rprint_lisst(DLinkedlisst lisst);

    int lookup_lisst(DLinkedlisst lisst, const string& ID);

    DLinkedlisst copy_lisst(DLinkedlisst);

    void delete_same_ID(DLinkedlisst &lisst);

    void destroy_lisst(DLinkedlisst &lisst);
};

#endif