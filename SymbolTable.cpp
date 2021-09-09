#include "SymbolTable.h"
#include "error.h"

IdentifierList::~IdentifierList() {
    auto *current = this->tail;
    while (current != nullptr) {
        auto *deleteNode = current;

        // SeT the pointer pointing from variable of the same name in parent scope to null
        if (deleteNode->prevOfSameType != nullptr) {
            deleteNode->prevOfSameType->nextOfSameType = nullptr;
        }

        current = current->prevInSameScope;

        delete deleteNode;
    }
    this->head = nullptr;
    this->tail = this->head;
}

auto IdentifierList::containIdentifierWithName(const string &name) const -> Identifier * {
    for (auto *identifier = this->head; identifier != nullptr; identifier = identifier->nextInSameScope) {
        if (identifier->id.name == name) {
            return &identifier->id;
        }
    }
    return nullptr;
}

auto IdentifierList::insert(const string &name, const IdentifierType type) -> void {
    auto *identifierNode = new IdentifierNode(Identifier(name, type), nullptr, this->tail);

    if (this->tail != nullptr) {
        identifierNode->prevInSameScope = this->tail;
        identifierNode->prevInSameScope->nextInSameScope = identifierNode;
    } else {
        this->head = identifierNode;
    }
    this->tail = identifierNode;
}

ScopeList::ScopeList() {
    this->addInnerScope();
}

auto ScopeList::addInnerScope() -> void {
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
    auto *current = this->innerMostScope;
    while (current != nullptr) {
        auto *deleteScope = current;
        current = current->parentScope;
        delete deleteScope;
    }
    this->innerMostScope = nullptr;
    this->globalScope = this->innerMostScope;
}

auto ScopeList::removeInnermostScope() -> void {
    if (this->globalScope != nullptr) {
        auto *temp = this->innerMostScope->parentScope;

        delete this->innerMostScope;

        this->innerMostScope = temp;
        if (this->innerMostScope == nullptr) {
            this->globalScope = nullptr;
        } else {
            this->innerMostScope->childScope = nullptr;
        }
    }
}

auto SymbolTable::run(const string &filename) -> void {

    std::ifstream fileInput(filename);

    std::string line;
    while (std::getline(fileInput, line)) {
        auto output = this->processLine(line);
        if (this->shouldPrint) {
#ifdef __EMSCRIPTEN__
            cout << output << std::endl;
#else
            std::cout << output << '\n';
#endif
        }
        this->shouldPrint = false;
    }
    this->detectUnclosedBlock();
    fileInput.close();
}

auto SymbolTable::processLine(const string &line) -> std::string {
    static const regex VALID_INSERT_REGEX(R"(^INSERT[ ]([a-z]\w*)[ ](string|number)$)");
    static const regex VALID_ASSIGN_REGEX(R"(^ASSIGN[ ]([a-z]\w*)[ ]([a-z]\w*|\d+|'[\dA-Za-z\s]*')$)");
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
        return { "success" };
    }

    if (std::regex_search(line, matches, VALID_ASSIGN_REGEX)) {
        const auto identifierName = matches[1];
        const auto value = matches[2];
        handleAssign(identifierName, value, line);

        this->shouldPrint = true;
        return { "success" };
    }

    if (std::regex_search(line, matches, VALID_LOOKUP_REGEX)) {
        const auto identifierName = matches[1];

        this->shouldPrint = true;
        return std::to_string(handleLookup(identifierName, line));
    }

    if (std::regex_match(line, VALID_BEGIN_REGEX)) {
        handleBegin();

        this->shouldPrint = false;
        return {};
    }

    if (std::regex_match(line, VALID_END_REGEX)) {
        handleEnd();

        this->shouldPrint = false;
        return {};
    }

    if (std::regex_match(line, VALID_PRINT_REGEX)) {
        auto result = handlePrint();

        this->shouldPrint = !result.empty();

        return result;
    }
    if (std::regex_match(line, VALID_REVERSE_PRINT_REGEX)) {
        auto result = handleReversePrint();

        this->shouldPrint = !result.empty();

        return result;
    }
    throw InvalidInstruction(line);
}

auto SymbolTable::handleInsert(const std::string &identifierName, const std::string &type, const std::string &line) const -> void {
    IdentifierType idType = type == "string" ? IdentifierType::string : IdentifierType::number;

    auto foundIdentifierInInnerMostScope = this->scopes.innerMostScope->containIdentifierWithName(identifierName) != nullptr;
    if (foundIdentifierInInnerMostScope) {
        throw Redeclared(line);
    }

    this->scopes.insert(identifierName, idType);

    // find the identifier node with the same name on the parents scope and SeT the next pointer
    auto *currentScope = this->scopes.innerMostScope->parentScope;
    while (currentScope != nullptr) {
        auto *idNode = currentScope->idList.head;
        while (idNode != nullptr) {
            if (idNode->id.name == identifierName) {
                idNode->nextOfSameType = this->scopes.innerMostScope->idList.tail;
                this->scopes.innerMostScope->idList.tail->prevOfSameType = idNode;
                return;
            }
            idNode = idNode->nextInSameScope;
        }
        currentScope = currentScope->parentScope;
    }
}

auto SymbolTable::handleAssign(const std::string &identifierName, const std::string &value, const std::string &line) const -> void {
    static const std::regex STRING_VALUE_REGEX(R"(^'[\dA-Za-z\s]+'$)");
    static const std::regex NUMBER_VALUE_REGEX(R"(^\d+$)");

    if (std::regex_match(value, NUMBER_VALUE_REGEX)) {
        auto type = IdentifierType::number;

        auto *id = this->scopes.containIdentifierWithName(identifierName);
        if (id == nullptr) {
            throw Undeclared(identifierName);
        }

        if (id->type != type) {
            throw TypeMismatch(line);
        }

        id->value = value;
        return;
    }
    if (std::regex_match(value, STRING_VALUE_REGEX)) {
        auto type = IdentifierType::string;

        auto *id = this->scopes.containIdentifierWithName(identifierName);
        if (id == nullptr) {
            throw Undeclared(identifierName);
        }
        if (id->type != type) {
            throw TypeMismatch(line);
        }
        id->value = value;
        return;
    }
    // value is identifier
    auto *currentScope = this->scopes.innerMostScope;
    Identifier *assignee = nullptr;
    Identifier *assigner = nullptr;
    while (currentScope != nullptr && (assignee == nullptr || assigner == nullptr)) {
        if (assignee == nullptr) {
            assignee = currentScope->containIdentifierWithName(identifierName);
        }
        if (assigner == nullptr) {
            assigner = currentScope->containIdentifierWithName(value);
        }
        currentScope = currentScope->parentScope;
    }

    if (assignee == nullptr) {
        throw Undeclared(identifierName);
    }

    if (assigner == nullptr) {
        throw Undeclared(value);
    }

    if (assignee->type != assigner->type) {
        throw TypeMismatch(line);
    }
    assignee->value = assigner->value;
}

auto SymbolTable::handleLookup(const string &identifierName, const std::string &line) const -> int {
    auto *currentScope = this->scopes.innerMostScope;
    while (currentScope != nullptr) {
        if (currentScope->containIdentifierWithName(identifierName) != nullptr) {
            return currentScope->level;
        }
        currentScope = currentScope->parentScope;
    }
    throw Undeclared(line);
}

auto SymbolTable::handleBegin() -> void {
    this->scopes.addInnerScope();
}

auto SymbolTable::handleEnd() -> void {
    if (this->scopes.innerMostScope->level <= 0) {
        throw UnknownBlock();
    }
    this->scopes.removeInnermostScope();
}

auto SymbolTable::handlePrint() const -> std::string {
    std::string output;
    for (auto *currentScope = this->scopes.globalScope; currentScope != nullptr; currentScope = currentScope->childScope) {
        for (auto *identifier = currentScope->idList.head; identifier != nullptr; identifier = identifier->nextInSameScope) {
            if (identifier->nextOfSameType == nullptr) {
                output += identifier->id.name;
                output += "//";
                output += std::to_string(currentScope->level);
                output += ' ';
            }
        }
    }
    if (output.empty()) {
        return output;
    }
    return { output.begin(), output.end() - 1 };
}

auto SymbolTable::handleReversePrint() const -> std::string {
    std::string output;
    for (auto *currentScope = this->scopes.innerMostScope;
         currentScope != nullptr;
         currentScope = currentScope->parentScope) {
        for (auto *identifier = currentScope->idList.tail;
             identifier != nullptr;
             identifier = identifier->prevInSameScope) {
            if (identifier->nextOfSameType == nullptr) {
                output += identifier->id.name;
                output += "//";
                output += std::to_string(currentScope->level);
                output += ' ';
            }
        }
    }
    if (output.empty()) {
        return output;
    }
    return { output.begin(), output.end() - 1 };
}

auto SymbolTable::detectUnclosedBlock() const -> void {
    auto level = this->scopes.innerMostScope->level;
    if (level != 0) {
        throw UnclosedBlock(level);
    }
}
