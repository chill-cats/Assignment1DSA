#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "main.h"

enum class IdentifierType {
    string,
    number,
};

class Identifier {
public:
    IdentifierType type = IdentifierType::string;
    std::string name;
    std::string value;

    Identifier() = default;

    Identifier(std::string name, IdentifierType type, std::string value = "");
};

class IdentifierNode {
public:
    Identifier id;
    IdentifierNode *nextInSameScope{nullptr};
    IdentifierNode *prevInSameScope{nullptr};

    IdentifierNode *prevOfSameType{nullptr};
    IdentifierNode *nextOfSameType{nullptr};
public:
    explicit IdentifierNode(Identifier id, IdentifierNode *next = nullptr, IdentifierNode *prev = nullptr);
};

class IdentifierList {
public:
    IdentifierNode *head{nullptr};
    IdentifierNode *tail{nullptr};

    ~IdentifierList();

    Identifier *containIdentifierWithName(const string &name) const;
};

class Scope {
public:
    IdentifierList idList;
    int level = 0;

    Scope *parentScope{nullptr};
    Scope *childScope{nullptr};

    explicit Scope(int level, Scope *parentScope = nullptr, Scope *childScope = nullptr);

};

class ScopeList {
public:
    Scope *globalScope{nullptr};
    Scope *innerMostScope{nullptr};

    void addInnerScope();

    void removeInnermostScope();

    ~ScopeList();
};

class SymbolTable {
public:
    ScopeList scopes;

    SymbolTable();

    void run(const string &filename);

    std::pair<string, bool> processLine(const std::string &str);

    void handleInsert(const std::string &identifierName, const std::string &type, const std::string &line) const;

    void handleAssign(const std::string &identifierName, const std::string &value, const std::string &line) const;

    int handleLookup(const std::string &identifierName) const;

    void handleBegin();

    void handleEnd();

    std::string handlePrint() const;

    std::string handleReversePrint() const;

    void handleCleanUp() const;

};

#endif