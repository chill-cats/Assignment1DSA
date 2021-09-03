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
    explicit IdentifierNode(Identifier id, IdentifierNode *next = nullptr, IdentifierNode *prev = nullptr,
                            IdentifierNode *nextOfSameType = nullptr, IdentifierNode *prevOfSameType = nullptr);
};

class IdentifierList {
public:
    IdentifierNode *head{nullptr};
    IdentifierNode *tail{nullptr};

    ~IdentifierList();

    [[nodiscard]] Identifier *containIdentifierWithName(const std::string &name) const;
    void insert(const std::string &name, const IdentifierType type);
};

class Scope {
public:
    IdentifierList idList;
    int level = 0;

    Scope *parentScope{nullptr};
    Scope *childScope{nullptr};

    explicit Scope(int level, Scope *parentScope = nullptr, Scope *childScope = nullptr);

    [[nodiscard]] inline Identifier *containIdentifierWithName(const std::string &name) const {
        return this->idList.containIdentifierWithName(name);
    }
    inline void insert(const std::string &name, const IdentifierType type) {
        this->idList.insert(name, type);
    }
};

class ScopeList {
public:
    Scope *globalScope{nullptr};
    Scope *innerMostScope{nullptr};

    void addInnerScope();
    void removeInnermostScope();

    inline void insert(const std::string &name, const IdentifierType type) const {
        this->innerMostScope->insert(name, type);
    }

    ~ScopeList();
};

class SymbolTable {
public:
    bool shouldPrint = false;
    ScopeList scopes;

    SymbolTable();
    void run(const string &filename);
    std::string processLine(const std::string &str);

    void handleInsert(const std::string &identifierName, const std::string &type, const std::string &line) const;
    void handleAssign(const std::string &identifierName, const std::string &value, const std::string &line) const;
    int handleLookup(const string &identifierName, const std::string &line) const;
    void handleBegin();
    void handleEnd();
    [[nodiscard]] std::string handlePrint() const;
    [[nodiscard]] std::string handleReversePrint() const;

    void detectUnclosedBlock() const;
};

#endif