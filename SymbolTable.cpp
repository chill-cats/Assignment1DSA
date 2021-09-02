#include "SymbolTable.h"

static std::string trim(const std::string &s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return {start, end + 1};
}

Identifier::Identifier(std::string name, IdentifierType type, std::string value) {
    this->value = std::move(value);
    this->type = type;
    this->name = std::move(name);
}

IdentifierNode::IdentifierNode(Identifier id, IdentifierNode *next, IdentifierNode *prev) {
    this->id = std::move(id);
    this->nextInSameScope = next;
    this->prevInSameScope = prev;
}


IdentifierList::~IdentifierList() {
    auto current = this->tail;
    while (current) {
        auto deleteNode = current;

        // set the pointer pointing from variable of the same name in parent scope to null
        if (deleteNode->prevOfSameType)
            deleteNode->prevOfSameType->nextOfSameType = nullptr;

        current = current->prevInSameScope;

        delete deleteNode;
        deleteNode = nullptr;
    }
    this->head = nullptr;
    this->tail = this->head;
}

Identifier *IdentifierList::containIdentifierWithName(const string &name) const {
    for (auto identifier = this->head; identifier != nullptr; identifier = identifier->nextInSameScope) {
        if (identifier->id.name == name)
            return &identifier->id;
    }
    return nullptr;
}


Scope::Scope(int level, Scope *parentScope, Scope *childScope) {
    this->level = level;
    this->parentScope = parentScope;
    this->childScope = childScope;
}

void ScopeList::addInnerScope() {
    auto *newScope = new Scope(0, this->innerMostScope, nullptr);

    if (this->innerMostScope != nullptr) {
        newScope->level = this->innerMostScope->level + 1;
        newScope->parentScope->childScope = newScope;
    } else {
        this->globalScope = newScope;
    }
    this->innerMostScope = newScope;
}

ScopeList::~ScopeList() {
    auto current = this->innerMostScope;
    while (current) {
        auto deleteScope = current;
        current = current->parentScope;
        delete deleteScope;
        deleteScope = nullptr;
    }
    this->innerMostScope = nullptr;
    this->globalScope = this->innerMostScope;
}

void ScopeList::removeInnermostScope() {
    if (this->globalScope != nullptr) {
        auto temp = this->innerMostScope->parentScope;

        delete this->innerMostScope;

        this->innerMostScope = temp;
        if (this->innerMostScope == nullptr) {
            this->globalScope = nullptr;
        } else {
            this->innerMostScope->childScope = nullptr;
        }
    }
}

void SymbolTable::run(const string &filename) {
    std::ifstream fileInput(filename);

#ifdef __EMSCRIPTEN__
    std::ifstream debugFile(filename);
    std::string str((std::istreambuf_iterator<char>(debugFile)),
                 std::istreambuf_iterator<char>());
    cerr << str << std::endl;
    cerr << "===========" << std::endl;
#endif

    std::string line;
    while (std::getline(fileInput, line)) {
        auto output = this->processLine(line);
        if (this->shouldPrint) {
#ifdef __EMSCRIPTEN__
            cout << output << std::endl;
#else
            std::cout << output << std::endl;
#endif
        }
        this->shouldPrint = false;
    }
    this->handleCleanUp();
    fileInput.close();
}

std::string SymbolTable::processLine(const string &line) {
    static const regex VALID_INSERT_REGEX(R"(^INSERT[ ]([a-z]\w*)[ ](string|number)$)");
    static const regex VALID_ASSIGN_REGEX(R"(^ASSIGN[ ]([a-z]\w*)[ ]([a-z]\w*|\d+|'[\dA-Za-z\s]+')$)"
    );
    static const regex VALID_LOOKUP_REGEX(R"(^LOOKUP[ ]([a-z]\w*)$)");
    static const regex VALID_BEGIN_REGEX(R"(^BEGIN$)");
    static const regex VALID_END_REGEX(R"(^END$)");
    static const regex VALID_PRINT_REGEX(R"(^PRINT$)");
    static const regex VALID_REVERSE_PRINT_REGEX(R"(^RPRINT$)");

    // Right now we have to match from once to 3 times to check the line is valid and
    // get the arguments for each command. Is there a way to both check the validity
    // and getting the arguments at the same time? 🥺🥺🥺🥺🥺🥺

    std::smatch matches;
    if (std::regex_search(line, matches, VALID_INSERT_REGEX)) {
        const auto identifierName = matches[1];
        const auto type = matches[2];
        handleInsert(identifierName, type, line);

        this->shouldPrint = true;
        return {"success"};

    } else if (std::regex_search(line, matches, VALID_ASSIGN_REGEX)) {
        const auto identifierName = matches[1];
        const auto value = matches[2];
        handleAssign(identifierName, value, line);

        this->shouldPrint = true;
        return {"success"};

    } else if (std::regex_search(line, matches, VALID_LOOKUP_REGEX)) {
        const auto identifierName = matches[1];

        this->shouldPrint = true;
        return std::to_string(handleLookup(identifierName, line));

    } else if (std::regex_match(line, VALID_BEGIN_REGEX)) {
        handleBegin();

        this->shouldPrint = false;
        return {};

    } else if (std::regex_match(line, VALID_END_REGEX)) {
        handleEnd();

        this->shouldPrint = false;
        return {};

    } else if (std::regex_match(line, VALID_PRINT_REGEX)) {
#ifndef __EMSCRIPTEN__
        auto result = trim(handlePrint());
        this->shouldPrint = !result.empty();
        return result;
#else
        this->shouldPrint = true;
        return trim(this->handlePrint());
#endif
    } else if (std::regex_match(line, VALID_REVERSE_PRINT_REGEX)) {
#ifndef __EMSCRIPTEN__
        auto result = trim(handleReversePrint());
        this->shouldPrint = !result.empty();
        return result;
#else
        this->shouldPrint = true;
        return trim(this->handleReversePrint());
#endif
    }
    throw InvalidInstruction(line);
}

void SymbolTable::handleInsert(const string &identifierName, const string &type, const string &line) const {
    // check type
    IdentifierType idType = type == "string" ? IdentifierType::string : IdentifierType::number;
    // create new node
    auto identifierNode = new IdentifierNode(Identifier(identifierName, idType));
    // find the innermost scope if the identifier already exist
    auto currentIdentifier = this->scopes.innerMostScope->idList.head;
    while (currentIdentifier) {
        if (currentIdentifier->id.name == identifierName) {
            delete identifierNode;
            throw Redeclared(line);
        }

        currentIdentifier = currentIdentifier->nextInSameScope;
    }
    // the identifier doest not exist in the innermost scope, insert the identifier to the innermost scope
    auto *idListOfInnerMostScope = &this->scopes.innerMostScope->idList;
    if (idListOfInnerMostScope->tail != nullptr) {
        identifierNode->prevInSameScope = idListOfInnerMostScope->tail;
        identifierNode->prevInSameScope->nextInSameScope = identifierNode;
    } else {
        idListOfInnerMostScope->head = identifierNode;
    }
    idListOfInnerMostScope->tail = identifierNode;

    // find the identifier node with the same name on the parents scope and set the next pointer
    auto currentScope = this->scopes.innerMostScope->parentScope;
    while (currentScope) {
        auto *idNode = currentScope->idList.head;
        while (idNode) {
            if (idNode->id.name == identifierName) {
                idNode->nextOfSameType = identifierNode;
                identifierNode->prevOfSameType = idNode;
                return;
            }
            idNode = idNode->nextInSameScope;
        }
        currentScope = currentScope->parentScope;
    }
}

void
SymbolTable::handleAssign(const std::string &identifierName, const std::string &value, const std::string &line) const {
    using std::regex;
    static const regex STRING_VALUE_REGEX(R"(^'[\dA-Za-z\s]+'$)");
    static const regex NUMBER_VALUE_REGEX(R"(^\d+$)");
    static const regex IDENTIFIER_VALUE_REGEX(R"(^[a-z]\w*$)");

    if (std::regex_match(value, NUMBER_VALUE_REGEX)) {
        auto *currentScope = this->scopes.innerMostScope;
        while (currentScope) {
            auto identifier = currentScope->idList.containIdentifierWithName(identifierName);
            if (identifier) {
                if (identifier->type != IdentifierType::number)
                    throw TypeMismatch(line);
                else {
                    identifier->value = value;
                    return;
                }
            }
            currentScope = currentScope->parentScope;
        }
        throw Undeclared(line);

    } else if (std::regex_match(value, STRING_VALUE_REGEX)) {
        auto *currentScope = this->scopes.innerMostScope;
        while (currentScope) {
            auto identifier = currentScope->idList.containIdentifierWithName(identifierName);
            if (identifier) {
                if (identifier->type != IdentifierType::string)
                    throw TypeMismatch(line);
                else {
                    identifier->value = value;
                    return;
                }
            }
            currentScope = currentScope->parentScope;
        }
        throw Undeclared(line);
    } else if (std::regex_match(value, IDENTIFIER_VALUE_REGEX)) {
        // value is identifier
        auto *currentScope = this->scopes.innerMostScope;
        Identifier *assignee = nullptr;
        Identifier *assigner = nullptr;
        while (currentScope && (assignee == nullptr || assigner == nullptr)) {
            if (assignee == nullptr) {
                assignee = currentScope->idList.containIdentifierWithName(identifierName);
            }
            if (assigner == nullptr) {
                assigner = currentScope->idList.containIdentifierWithName(value);
            }
            currentScope = currentScope->parentScope;
        }
        if (assigner == nullptr || assignee == nullptr) {
            throw Undeclared(line);
        }
        if (assignee->type != assigner->type) {
            throw TypeMismatch(line);
        }
        assignee->value = assigner->value;
        return;
    }

    throw InvalidInstruction(line);
}

int SymbolTable::handleLookup(const string &identifierName, const std::string &line) const {
    auto currentScope = this->scopes.innerMostScope;
    while (currentScope) {
        if (currentScope->idList.containIdentifierWithName(identifierName)) {
            return currentScope->level;
        }
        currentScope = currentScope->parentScope;
    }
    throw Undeclared(line);
}

void SymbolTable::handleBegin() {
    this->scopes.addInnerScope();
}

void SymbolTable::handleEnd() {
    if (this->scopes.innerMostScope->level <= 0) {
        throw UnknownBlock();
    }
    this->scopes.removeInnermostScope();
}

std::string SymbolTable::handlePrint() const {
    std::string output;
    for (auto currentScope = this->scopes.globalScope;
         currentScope != nullptr; currentScope = currentScope->childScope) {
        for (auto identifier = currentScope->idList.head;
             identifier != nullptr; identifier = identifier->nextInSameScope) {
            if (identifier->nextOfSameType == nullptr) {
                output += identifier->id.name;
                output += "//";
                output += std::to_string(currentScope->level);
                output += ' ';
            }
        }
    }
    return output;
}

std::string SymbolTable::handleReversePrint() const {
    std::string output;
    for (auto currentScope = this->scopes.innerMostScope;
         currentScope != nullptr; currentScope = currentScope->parentScope) {
        for (auto identifier = currentScope->idList.tail;
             identifier != nullptr; identifier = identifier->prevInSameScope) {
            if (identifier->nextOfSameType == nullptr) {
                output += identifier->id.name;
                output += "//";
                output += std::to_string(currentScope->level);
                output += ' ';
            }
        }
    }
    return output;
}

void SymbolTable::handleCleanUp() const {
    auto level = this->scopes.innerMostScope->level;
    if (level != 0)
        throw UnclosedBlock(level);
}

SymbolTable::SymbolTable() {
    this->scopes.addInnerScope();
}
