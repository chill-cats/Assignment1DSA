#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};

class Symbol
{
    string identifier  ;
    string type ;
    int level;
public:
    Symbol(string, string, int);
    Symbol()
    {
        this->identifier = " ";
        this->type = " ";
        this->level = 0;
    }
    string getIdentifier() { return this->identifier; };
    string getType() { return this->type; };
    int getLevel() { return this->level; };
    void operator = (const Symbol& b)
    {
        this->identifier = b.identifier;
        this->level = b.level;
        this->type = b.type;
    }
};



#endif