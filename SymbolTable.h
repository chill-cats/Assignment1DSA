#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Identifier{
public: 
    string type;
    string name;
    string value;
    Identifier();
    ~Identifier();
    Identifier(string name, string type, string value);
};

class Idn_Node{
public:
    Identifier data; //Data in node
    int node_level;
    Idn_Node *next; //Next pointer
    Idn_Node *prev;
    Idn_Node();
    ~Idn_Node();
    Idn_Node(Identifier new_data);
    void setData(int data); //Assign new value for variable
    void setLevel(int level);
    bool enable;
};


class Idn_List{
public:
    Idn_List();
    ~Idn_List();
    Idn_Node *head; //head of a list
    Idn_Node *tail; //tail of a list
    Idn_List *parent;
    Idn_List *child;
    int size;
    int level;
    void append(Idn_Node *node);
    void destroy_list(); //Clean
    void destroy_node(Idn_Node *tobe_del); //Xoa node
    void printForward(); //PRINT
    void printBackward(); //RPRINT
    //void set_value(string name, string val);
    bool find(string name);

    Idn_Node *getNode(string name);
    void printout(); // Testing 

};

class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();
    int numBlock;
    Idn_List *global_list;
    Idn_List *curList;
    Idn_List *track_list;
    //MAIN METHODS
    void run(string filename);  
    void insert(string line);
    void lookup(string line);
    void assign(string line, int type);
    void print();
    void rprint();
    void new_scope();
    void end_scope();
    //HANDLING EXCEPTION
    void handle_exception_insert(string line);
    int handle_exception_assign(string line);
    void handle_exception_print(string line);
    void handle_exception_rprint(string line);
    void handle_end_file();
    
    void cleanup();
    pair<string,int> process(string line);
    void printList(); //Testing
};


#endif