#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

using namespace std;

class identifier_name {
public:
    identifier_name(): ID(), type(), value(""), level(0) {}
    identifier_name(string ID, string type, string value, int level): ID(ID), type(type), value(value), level(level) {}
    string ID;
    string type;
    string value;
    int level;
};

class identifier_node {
public:
    identifier_node(): data(identifier_name()), next(nullptr), prev(nullptr) {}
    identifier_node(identifier_name data, identifier_node* next, identifier_node* prev): data(data), next(next), prev(nullptr) {}

    identifier_name data;
    identifier_node* next;
    identifier_node* prev;
};

class DLinkedLisst {
public:
    identifier_node* head;
    identifier_node* tail;

public:
    DLinkedLisst(): head(nullptr), tail(nullptr) {}

    DLinkedLisst(DLinkedLisst *other) {
        for (auto *h = other->head; h != nullptr; h = h->next) {
            identifier_name h1(h->data.ID, h->data.type, h->data.value, h->data.level);
            insert_lisst(h1);
        }
    }

    ~DLinkedLisst() {
        identifier_node *temp = this->head;
        while (temp != nullptr) {
            auto toBeDeletedNode = temp;
            temp = temp->next;
            delete toBeDeletedNode;
        }
    }

public:
    bool insert_lisst(const identifier_name& newID);

    static string type_of_value(const string& value);

    string assign_lisst(const string& ID, const string& value) const;

    void end_block(int level);

    DLinkedLisst delete_same_ID();

    static void print_lisst(const DLinkedLisst& lisst) ;

    static void rprint_lisst(const DLinkedLisst& lisst) ;

    int lookup_lisst(const string& ID) const;

};

class SymbolTable {
public:
    static void run(const string& filename);
};

#endif