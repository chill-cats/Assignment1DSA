#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Identifier{
public: 
    string type;
    string name;
    string value;
    Identifier();
    Identifier(string name, string type, string value);
};

class Idn_Node{
public:
    Identifier data; //Data in node
    Idn_Node *next; //Next pointer
    Idn_Node *prev;
    Idn_Node();
    Idn_Node(Identifier new_data);
    void setData(int data); //Assign new value for variable
};


class Idn_List{
public:
    Idn_List();
    Idn_Node *head; //head of a list
    Idn_Node *tail; //tail of a list
    Idn_List *parent;
    Idn_List *child;
    int size;
    int level;
    void insert_to_list(Idn_Node *node);
    //void set_value(string name, string val);
    bool find(string name);
    Idn_Node *getNode(string name);
};

class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();
    Idn_List global_list;
    Idn_List *curList;

    //MAIN METHODS
    void run(string filename);
    void insert(string line);
    void lookup(string line);
    void assign(string line, int type);
    void print(string line);
    void rprint(string line);
    //HANDLING EXCEPTION
    void handle_exception_insert(string line);
    int handle_exception_assign(string line);
    void handle_exception_begin(string line);
    void handle_exception_lookup(string line);
    void handle_exception_print(string line);
    void handle_exception_rprint(string line);
    
    pair<string,int> process(string line);
    void printList(); //just for testing
};


#endif