#include "SymbolTable.h"

using namespace std;

bool SymbolTable::check_valid_name(string s) {
    if (s[0] < 'a' || s[0] > 'z') return false;
    for (int i = 1; i < (int) s.length(); i++) {
        if (s[i] != ' ' && s[i] != '_') {
            if ((s[i] < 'A' || s[i] > 'Z') && (s[i] < 'a' || s[i] > 'z') && (s[i] < '0' || s[i] > '9')) return false;
        }
    }
    return true;
}

int SymbolTable::check_tok(string s) {
    int count = 0;
    if (s == "BEGIN" || s == "END" || s == "PRINT" || s == "RPRINT") return 1;
    else {
        string s1;
        for (int i = 0; i < (int) s.length(); i++) {
            if (s[i] != ' ') s1.push_back(s[i]);
            else break;
        }
        if (s1 != "INSERT" && s1 != "ASSIGN" && s1 != "LOOKUP") return 0;
        else {
            for (int i = 1; i < (int) s.length(); i++) {
                if (s[i] == ' ' && i != (int) s.length() - 1 && s[i] != s[i - 1]) count++;
                else if (s[i] == ' ' && (i == (int) s.length() - 1 || s[i] == s[i - 1])) return 0;
            }
            if (count > 2) return 0;
            return ++count;
        }
    }
}

bool SymbolTable::check_valid(string s) {
    if (check_tok(s) != 0) {
        if (check_tok(s) == 1) return true;
        int n = check_tok(s);
        auto* s1 = new string[n];
        int a = 0;
        for (int i = 0; i < (int) s.length(); i++) {
            if (s[i] != ' ') s1[a].push_back(s[i]);
            if (s[i] == ' ') a++;
        }
        if (s1[0] == "INSERT" || s1[0] == "ASSIGN") {
            if (check_valid_name(s1[1]) && check_tok(s) == 3) {
                if (s1[0] == "INSERT" && s1[2] != "number" && s1[2] != "string") return false;
                return true;
            }
        } else if (s1[0] == "LOOKUP" && (check_valid_name(s1[1]) && check_tok(s) == 2)) return true;
    } else return false;
    return false;
}

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

void SymbolTable::run(string filename) {
    DLinkedList list;
    int level = 0, count_begin = 0, count_end = 0, n = count_line(filename), count_line = 0;
    ifstream myfile;
    myfile.open(filename);
    if (myfile.is_open()) {
        string s;
        while (getline(myfile, s)) {
            count_line++;
            if (check_valid(s)) {
                int n = check_tok(s);
                auto* s1 = new string[n];
                int a = 0;
                for (int i = 0; i < (int) s.length(); i++) {
                    if (s[i] != ' ') s1[a].push_back(s[i]);
                    if (s[i] == ' ') a++;
                }
                if (s1[0] == "INSERT") {
                    identifier_name newID(s1[1], s1[2], "", level);
                    if (insert_list(list, newID)) {
                        list.size++;
                        cout << "success" << endl;
                    } else {
                        Redeclared I(s);
                        cout << I.what();
                        break;
                    }
                } else if (s1[0] == "ASSIGN") {
                    string as = assign(list, s1[1], s1[2]);
                    if (as == "success") cout << "success" << endl;
                    else if (as == "mismatch") {
                        TypeMismatch I(s);
                        cout << I.what();
                        break;
                    } else if (as == "undeclared") {
                        Undeclared I(s);
                        cout << I.what();
                        break;
                    } else if (as == "invalid") {
                        InvalidInstruction I(s);
                        cout << I.what();
                        break;
                    }
                } else if (s1[0] == "BEGIN") {
                    level++;
                    count_begin++;
                } else if (s1[0] == "END") {
                    end(list, 1);
                    level--;
                    count_end++;
                    if (count_end > count_begin) {
                        UnknownBlock I;
                        cout << I.what();
                        break;
                    }
                } else if (s1[0] == "PRINT") {
                    print(list);
                } else if (s1[0] == "RPRINT") {
                    rprint(list);
                } else if (s1[0] == "LOOKUP") {
                    if (lookup(list, s1[1]) == -1) {
                        Undeclared I(s);
                        cout << I.what();
                        break;
                    } else cout << lookup(list, s1[1]) << endl;
                }
            } else {
                InvalidInstruction I(s);
                cout << I.what();
                break;
            }
        }
        if (count_line == n) {
            if (count_begin > count_end) {
                UnclosedBlock s1 = UnclosedBlock(level);
                UnclosedBlock I(s1);
                cout << I.what();
            }
        }
    }
    myfile.close();
}

bool SymbolTable::insert_list(DLinkedList &list, identifier_name element) {
    auto *h = list.head;
    auto *h1 = new identifier_node;
    h1->data = element;
    if (list.head == nullptr) {
        list.head = h1;
        h1->next = nullptr;
        h1->prev = nullptr;
        list.tail = list.head;
        return true;
    }
    while (h->next != nullptr) {
        if (h->data.ID == element.ID && h->data.level == element.level) return false;
        h = h->next;
    }
    if (h->data.ID == element.ID && h->data.level == element.level) return false;
    h->next = h1;
    h1->next = nullptr;
    h1->prev = h;
    list.tail = h1;
    return true;
}

string SymbolTable::type_of_value(string value) {
    string error = "invalid";
    if (value[0] != '\'') {
        int i, n = value.length();
        for (i = 0; i < n; i++) {
            if (value[i] > '9' || value[i] < '0') break;
        }
        if (i >= n) return "number";
        if (check_valid_name(value)) return "id";
    } else {
        int n = value.length();
        if (n == 1) return error;
        if (n == 2) {
            if (value[1] == '\'') return "string";
            else return error;
        }
        for (int i = 1; i < n - 1; i++) {
            if (value[i] != ' ') {
                if ((value[i] < 'A' || value[i] > 'Z') && (value[i] < 'a' || value[i] > 'z') && (value[i] < '0' || value[i] > '9')) return error;
            }
        }
        if (value[n - 1] == '\'') return "string";
    }
    return error;
}

string SymbolTable::assign(DLinkedList &list, string ID, string value) {
    identifier_node *h = list.tail;
    if (type_of_value(value) == "invalid") return "invalid";
    while (h) {
        if (h->data.ID == ID) {
            if (type_of_value(value) == "id") {
                auto *h1 = list.tail;
                while (h1) {
                    if (h1->data.ID == value) {
                        if (h1->data.type == h->data.type) {
                            h->data.value = value;
                            return "success";
                        } else return "mismatch";
                    }
                    h1 = h1->prev;
                }
                return "undeclared";
            }
            if (type_of_value(value) == h->data.type) {
                h->data.value = value;
                return "success";
            } else return "mismatch";
        }
        h = h->prev;
    }
    return "undeclared";
}

void SymbolTable::end(DLinkedList &list, int level) {
    if (list.head == nullptr) return;
    auto *h = list.head;
    while (h) {
        if (h->data.level == level) {
            if (list.head == list.tail) {
                list.tail = nullptr;
                list.head = nullptr;
                free(h);
                return;
            }
            if (h == list.head) {
                list.head = list.head->next;
                free(h);
                list.head->prev = nullptr;
                h = list.head;
            } else if (h == list.tail) {
                list.tail = list.tail->prev;
                list.tail->next = nullptr;
                free(h);
                break;
            } else {
                auto *h2 = h->prev;
                h2->next = h->next;
                h->next->prev = h2;
                free(h);
                h = h2->next;
            }
        } else h = h->next;
    }
}

int SymbolTable::count_ID(DLinkedList list, string ID) {
    auto *h = list.head;
    int count = 0;
    while (h) {
        if (h->data.ID == ID) count++;
        h = h->next;
    }
    return count;
}

SymbolTable::DLinkedList SymbolTable::copy(DLinkedList list) {
    if (list.head == nullptr) return list;
    DLinkedList list1;
    identifier_node *h = list.head;
    while (h) {
        identifier_name h1(h->data.ID, h->data.type, h->data.value, h->data.level);
        insert_list(list1, h1);
        h = h->next;
    }
    return list1;
}

void SymbolTable::delete_same_ID(DLinkedList &list) {
    if (list.head == nullptr) return;
    auto h = list.head;
    while (h) {
        int a = count_ID(list, h->data.ID);
        if (a > 1) {
            auto *h1 = h;
            string cur = h->data.ID;
            while (a > 1) {
                if (h1->data.ID == cur) {
                    if (h == list.head) {
                        h1 = h1->next;
                        list.head = list.head->next;
                        free(h);
                        list.head->prev = nullptr;
                        h = h1;
                        a--;
                    } else {
                        h1 = h1->next;
                        auto *h2 = h->prev;
                        h2->next = h->next;
                        h->next->prev = h2;
                        free(h);
                        h = h1;
                        a--;
                    }
                }
                h1 = h1->next;
                if (h1 == nullptr) break;
                h = h1;
            }
        }
        h = h->next;
    }
}

void SymbolTable::print(DLinkedList list) {
    if (list.head == nullptr) return;
    DLinkedList list1 = copy(list);
    delete_same_ID(list1);
    auto h = list1.head;
    while (h->next) {
        cout << h->data.ID << "//" << h->data.level << " ";
        h = h->next;
    }
    cout << h->data.ID << "//" << h->data.level;
    cout << endl;
}

void SymbolTable::rprint(DLinkedList list) {
    if (list.head == nullptr) return;
    DLinkedList list1 = copy(list);
    delete_same_ID(list1);
    auto h = list1.tail;
    while (h->prev) {
        cout << h->data.ID << "//" << h->data.level << " ";
        h = h->prev;
    }
    cout << h->data.ID << "//" << h->data.level;
    cout << endl;
}

int SymbolTable::lookup(DLinkedList list, string ID) {
    identifier_node *h = list.tail;
    while (h) {
        if (h->data.ID == ID) return h->data.level;
        h = h->prev;
    }
    return -1;
}