#include "SymbolTable.h"

using namespace std;

int SymbolTable::count_line(string filename) {
    int count = 0;
    ifstream myfile;
    myfile.open(filename);
    if (myfile.is_open()) {
        string s;
        while (getline(myfile, s)) {
            count++;
        }
    }
    myfile.close();
    return count;
}

void SymbolTable::destroy_lisst(DLinkedlisst &lisst) {
    identifier_node *temp = lisst.head;
    while (temp != nullptr) {
        auto toBeDeletedNode = temp;
        temp = temp->next;
        delete toBeDeletedNode;
    }
}

void SymbolTable::run(const string& filename) {
    DLinkedlisst lisst;
    int level = 0, count_begin = 0, count_end = 0, line = count_line(filename),
            count_line = 0;
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
            count_line++;
            if (regex_match(s, data, insert)) {
                identifier_name newID(data.str(1), data.str(2), "", level);
                if (insert_lisst(lisst, newID)) {
                    lisst.size++;
                    cout << "success" << endl;
                } else {
                    destroy_lisst(lisst);
                    throw Redeclared(s);
                }
            } else if (regex_match(s, data, assign)) {
                string as = assign_lisst(lisst, data[1], data[2]);
                if (as == "success")
                    cout << "success" << endl;
                else if (as == "mismatch") {
                    destroy_lisst(lisst);
                    throw TypeMismatch(s);
                } else if (as == "undeclared") {
                    destroy_lisst(lisst);
                    throw Undeclared(s);
                } else if (as == "invalid") {
                    destroy_lisst(lisst);
                    throw InvalidInstruction(s);
                }
            } else if (regex_match(s, begin)) {
                level++;
                count_begin++;
            } else if (regex_match(s, end)) {
                count_end++;
                if (count_end > count_begin) {
                    destroy_lisst(lisst);
                    throw UnknownBlock();
                }
                end_lisst(lisst, level);
                level--;
            } else if (regex_match(s, print))
                print_lisst(lisst);
            else if (regex_match(s, rprint))
                rprint_lisst(lisst);
            else if (regex_match(s, data, lookup)) {
                if (lookup_lisst(lisst, data[1]) == -1) {
                    destroy_lisst(lisst);
                    throw Undeclared(s);
                } else {
                    cout << lookup_lisst(lisst, data[1]) << endl;
                }
            } else {
                destroy_lisst(lisst);
                throw InvalidInstruction(s);
            }
        }
        if (count_line == line) {
            if (count_begin > count_end) {
                destroy_lisst(lisst);
                throw UnclosedBlock(level);
            }
        }
    }
    myfile.close();
    destroy_lisst(lisst);
}

bool SymbolTable::insert_lisst(DLinkedlisst &lisst,
                               const identifier_name &element) {
    auto *h1 = new identifier_node;
    h1->data = element;
    if (lisst.head == nullptr) {
        lisst.head = h1;
        h1->next = nullptr;
        h1->prev = nullptr;
        lisst.tail = lisst.head;
        return true;
    }
    auto *h = lisst.head;
    while (h->next != nullptr) {
        if (h->data.ID == element.ID && h->data.level == element.level) {
            delete h1;
            return false;
        }
        h = h->next;
    }
    if (h->data.ID == element.ID && h->data.level == element.level) {
        delete h1;
        return false;
    }
    h->next = h1;
    h1->next = nullptr;
    h1->prev = h;
    lisst.tail = h1;
    return true;
}

string SymbolTable::type_of_value(string value) {
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

string SymbolTable::assign_lisst(DLinkedlisst &lisst, const string &ID,
                           const string &value) {
    auto *h = lisst.tail;
    while (h) {
        if (h->data.ID == ID) {
            if (type_of_value(value) == "id") {
                auto *h1 = lisst.tail;
                while (h1) {
                    if (h1->data.ID == value) {
                        if (h1->data.type == h->data.type) {
                            h->data.value = value;
                            return "success";
                        } else
                            return "mismatch";
                    }
                    h1 = h1->prev;
                }
                return "undeclared";
            }
            if (type_of_value(value) == h->data.type) {
                h->data.value = value;
                return "success";
            } else
                return "mismatch";
        }
        h = h->prev;
    }
    return "undeclared";
}

void SymbolTable::end_lisst(DLinkedlisst &lisst, int level) {
    if (lisst.head == nullptr)
        return;

    auto *h = lisst.head;
    while (h) {
        if (h->data.level == level) {
            if (lisst.head == lisst.tail) {
                delete lisst.head;
                lisst.tail = nullptr;
                lisst.head = nullptr;

                return;
            }
            if (h == lisst.head) {
                lisst.head = lisst.head->next;
                delete h;
                lisst.head->prev = nullptr;
                h = lisst.head;
            } else if (h == lisst.tail) {
                lisst.tail = lisst.tail->prev;
                lisst.tail->next = nullptr;
                delete h;
                break;
            } else {
                auto *h2 = h->prev;
                h2->next = h->next;
                h->next->prev = h2;
                delete h;
                h = h2->next;
            }
        } else
            h = h->next;
    }
}

int SymbolTable::count_ID(DLinkedlisst lisst, const string &ID) {
    if (lisst.head == nullptr)
        return 0;
    auto *h = lisst.head;
    int count = 0;
    while (h) {
        if (h->data.ID == ID)
            count++;
        h = h->next;
    }
    return count;
}

SymbolTable::DLinkedlisst SymbolTable::copy_lisst(DLinkedlisst lisst) {
    if (lisst.head == nullptr)
        return lisst;
    DLinkedlisst lisst1;
    auto *h = lisst.head;
    while (h) {
        identifier_name h1(h->data.ID, h->data.type, h->data.value, h->data.level);
        insert_lisst(lisst1, h1);
        h = h->next;
    }
    return lisst1;
}

void SymbolTable::delete_same_ID(DLinkedlisst &lisst) {
    if (lisst.head == nullptr)
        return;
    auto *h = lisst.head;
    while (h) {
        int a = count_ID(lisst, h->data.ID);
        if (a > 1) {
            auto *h1 = h;
            string cur = h->data.ID;
            while (a > 1) {
                if (h1->data.ID == cur) {
                    if (h == lisst.head) {
                        h1 = h1->next;
                        lisst.head = lisst.head->next;
                        delete h;
                        lisst.head->prev = nullptr;
                        h = h1;
                        a--;
                    } else {
                        h1 = h1->next;
                        auto *h2 = h->prev;
                        h2->next = h->next;
                        h->next->prev = h2;
                        delete h;
                        h = h1;
                        a--;
                    }
                }
                h1 = h1->next;
                if (h1 == nullptr)
                    break;
                h = h1;
            }
        }
        h = h->next;
    }
}

void SymbolTable::print_lisst(DLinkedlisst lisst) {
    if (lisst.head == nullptr)
        return;
    DLinkedlisst lisst1 = copy_lisst(lisst);
    delete_same_ID(lisst1);
    auto h = lisst1.head;
    while (h->next) {
        cout << h->data.ID << "//" << h->data.level << " ";
        h = h->next;
    }
    cout << h->data.ID << "//" << h->data.level;
    cout << endl;
    destroy_lisst(lisst1);
}

void SymbolTable::rprint_lisst(DLinkedlisst lisst) {
    if (lisst.head == nullptr)
        return;
    DLinkedlisst lisst1 = copy_lisst(lisst);
    delete_same_ID(lisst1);
    auto h = lisst1.tail;
    while (h->prev) {
        cout << h->data.ID << "//" << h->data.level << " ";
        h = h->prev;
    }
    cout << h->data.ID << "//" << h->data.level;
    cout << endl;
    destroy_lisst(lisst1);
}

int SymbolTable::lookup_lisst(DLinkedlisst lisst, const string &ID) {
    identifier_node *h = lisst.tail;
    while (h) {
        if (h->data.ID == ID)
            return h->data.level;
        h = h->prev;
    }
    return -1;
}
