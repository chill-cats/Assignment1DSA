#include "SymbolTable.h"

using namespace std;

void SymbolTable::run(const string& filename) {
    DLinkedLisst lisst;
    int level = 0;
    regex insert(R"(^INSERT[ ]([a-z]\w*)[ ](string|number)$)");
    regex assign(R"(^ASSIGN[ ]([a-z]\w*)[ ]([a-z]\w*|\d+|'[a-zA-Z\d ]*')$)");
    regex begin(R"(^BEGIN$)");
    regex end(R"(^END$)");
    regex print(R"(^PRINT$)");
    regex rprint(R"(^RPRINT$)");
    regex lookup(R"(^LOOKUP[ ]([a-z]\w*))");
    ifstream myfile;
    myfile.open(filename);
    if (myfile.is_open()) {
        string s;
        while (getline(myfile, s)) {
            smatch data;

            if (regex_match(s, data, insert)) {
                identifier_name newID(data[1], data[2], "", level);
                if (lisst.insert_lisst(newID)) {
                    cout << "success" << endl;
                } else {
                    throw Redeclared(s);
                }

            } else if (regex_match(s, data, assign)) {
                string as = lisst.assign_lisst(data[1], data[2]);
                if (as == "success") {
                    cout << "success" << endl;
                } else if (as == "mismatch") {
                    throw TypeMismatch(s);
                } else if (as == "undeclared") {
                    throw Undeclared(s);
                } else if (as == "invalid") {
                    throw InvalidInstruction(s);
                }

            } else if (regex_match(s, begin)) {
                level++;

            } else if (regex_match(s, end)) {
                if (level == 0) {
                    throw UnknownBlock();
                }
                lisst.end_block(level);
                level--;

            } else if (regex_match(s, print)) {
                DLinkedLisst::print_lisst(lisst.delete_same_ID());

            } else if (regex_match(s, rprint)) {
                DLinkedLisst::rprint_lisst(lisst.delete_same_ID());

            } else if (regex_match(s, data, lookup)) {
                if (lisst.lookup_lisst(data[1]) == -1) {
                    throw Undeclared(s);
                } else {
                    cout << lisst.lookup_lisst(data[1]) << endl;
                }

            } else {
                throw InvalidInstruction(s);
            }
        }
        if (level != 0) {
            throw UnclosedBlock(level);
        }
    }
    myfile.close();
}

bool DLinkedLisst::insert_lisst(const identifier_name& newID) {
    auto *new_ID = new identifier_node;
    new_ID->data = newID;
    if (this->head == nullptr) {
        this->head = new_ID;
        this->tail = new_ID;
        return true;
    }

    for (auto *h = this->head; h != nullptr; h = h->next) {
        if (h->data.ID == newID.ID && h->data.level == newID.level) {
            delete new_ID;
            return false;
        }
    }
    this->tail->next = new_ID;
    new_ID->prev = this->tail;
    this->tail = new_ID;
    return true;
}

string DLinkedLisst::type_of_value(const string& value) {
    regex number(R"(\d+)");
    regex string(R"('[a-zA-Z\d ]*')");
    regex id(R"([a-z]\w*)");
    if (regex_match(value, number))
        return "number";
    if (regex_match(value, string))
        return "string";
    if (regex_match(value, id))
        return "id";
    return "invalid";
}

string DLinkedLisst::assign_lisst(const string& ID, const string& value) const {
    for (auto *h = this->tail; h != nullptr; h = h->prev) {
        if (h->data.ID == ID) {
            if (type_of_value(value) == "id") {
                for (auto *h1 = this->tail; h1 != nullptr; h1 = h1->prev) {
                    if (h1->data.ID == value) {
                        if (h1->data.type == h->data.type) {
                            h->data.value = value;
                            return "success";
                        } else {
                            return "mismatch";
                        }
                    }
                }
                return "undeclared";
            }
            if (type_of_value(value) == h->data.type) {
                h->data.value = value;
                return "success";
            } else {
                return "mismatch";
            }
        }
    }
    return "undeclared";
}

void DLinkedLisst::end_block(int level) {
    if (this->head == nullptr) {
        return;
    }
    auto *h = this->tail;
    while (h->data.level == level) {
        if (this->head == this->tail) {
            delete h;
            this->head = nullptr;
            this->tail = nullptr;
            return;
        }
        h = h->prev;
        delete this->tail;
        this->tail = h;
        this->tail->next = nullptr;
    }
}

DLinkedLisst DLinkedLisst::delete_same_ID() {
    DLinkedLisst lisst1(this);
    if (this->head == nullptr) {
        return lisst1;
    }

    for (auto *h = lisst1.tail; h != nullptr; h = h->prev) {
        for (auto *h1 = h->prev; h1 != nullptr; h1 = h1->prev) {
            if (h1->data.ID == h->data.ID) {
                if (h1 == lisst1.head) {
                    lisst1.head = lisst1.head->next;
                    lisst1.head->prev = nullptr;
                    delete h1;
                    break;
                }
                auto *h2 = h1;
                h1 = h1->next;
                h1->prev = h2->prev;
                h2->prev->next = h1;
                delete h2;
            }
        }
    }
    return lisst1;
}

void DLinkedLisst::print_lisst(const DLinkedLisst& lisst) {
    if (lisst.head == nullptr) {
        return;
    }

    auto h = lisst.head;
    while (h->next) {
        cout << h->data.ID << "//" << h->data.level << " ";
        h = h->next;
    }
    cout << h->data.ID << "//" << h->data.level;
    cout << endl;
}

void DLinkedLisst::rprint_lisst(const DLinkedLisst& lisst) {
    if (lisst.head == nullptr) {
        return;
    }

    auto h = lisst.tail;
    while (h->prev) {
        cout << h->data.ID << "//" << h->data.level << " ";
        h = h->prev;
    }
    cout << h->data.ID << "//" << h->data.level;
    cout << endl;
}

int DLinkedLisst::lookup_lisst(const string& ID) const {
    if (this->head == nullptr) {
        return -1;
    }
    for (auto *h = this->tail; h != nullptr; h = h->prev) {
        if (h->data.ID == ID) {
            return h->data.level;
        }
    }
    return -1;
}
