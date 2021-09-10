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

    Identifier(std::string name, IdentifierType type, std::string value = "") : type(type), name(std::move(name)),
                                                                                value(std::move(value)) {}
};

class IdentifierNode {
  public:
    Identifier id;
    std::unique_ptr<IdentifierNode> nextInSameScope;
    IdentifierNode *prevInSameScope{ nullptr };

    IdentifierNode *prevOfSameType{ nullptr };
    IdentifierNode *nextOfSameType{ nullptr };

    explicit IdentifierNode(Identifier id, IdentifierNode *next = nullptr, IdentifierNode *prev = nullptr, IdentifierNode *nextOfSameType = nullptr, IdentifierNode *prevOfSameType = nullptr) : id(std::move(id)), nextInSameScope{ next }, prevInSameScope(prev), prevOfSameType(prevOfSameType), nextOfSameType(nextOfSameType) {}
};

class IdentifierList {
  public:
    std::unique_ptr<IdentifierNode> head;
    IdentifierNode *tail{ nullptr };

    ~IdentifierList();

    [[nodiscard]] Identifier *containIdentifierWithName(const std::string &name) const;

    void insert(const std::string &name, IdentifierType type);
};

class Scope {
  public:
    IdentifierList idList;
    int level = 0;

    Scope *parentScope{ nullptr };
    std::unique_ptr<Scope> childScope;

    explicit Scope(int level, Scope *parentScope = nullptr, Scope *childScope = nullptr) : level{ level },
                                                                                           parentScope{ parentScope },
                                                                                           childScope{ childScope } {}

    [[nodiscard]] inline Identifier *containIdentifierWithName(const std::string &name) const {
        return this->idList.containIdentifierWithName(name);
    }

    inline void insert(const std::string &name, const IdentifierType type) {
        this->idList.insert(name, type);
    }
};

class ScopeList {
  public:
    std::unique_ptr<Scope> globalScope;
    Scope *innerMostScope{ nullptr };

    void addInnerScope();

    void removeInnermostScope();

    ScopeList();

    inline void insert(const std::string &name, const IdentifierType type) const {
        this->innerMostScope->insert(name, type);
    }

    [[nodiscard]] inline Identifier *containIdentifierWithName(const std::string &name) const {
        for (auto *scope = innerMostScope; scope != nullptr; scope = scope->parentScope) {
            auto *id = scope->containIdentifierWithName(name);
            if (id != nullptr) {
                return id;
            }
        }
        return nullptr;
    }

    ~ScopeList();
};

class SymbolTable {
  public:
    bool shouldPrint = false;
    ScopeList scopes;

    auto run(const string &filename) -> void;

    auto processLine(const std::string &line) -> std::string;

    auto
      handleInsert(const std::string &identifierName, const std::string &type, const std::string &line) const -> void;

    auto
      handleAssign(const std::string &identifierName, const std::string &value, const std::string &line) const -> void;

    [[nodiscard]] auto handleLookup(const string &identifierName, const std::string &line) const -> int;

    auto handleBegin() -> void;

    auto handleEnd() -> void;

    [[nodiscard]] auto handlePrint() const -> std::string;

    [[nodiscard]] auto handleReversePrint() const -> std::string;

    void detectUnclosedBlock() const;
};

#endif
