#include "SymbolTable.h"

void SymbolTable::checkRPRINT(List& list, string S, int line, int count_line) {
    if (list.size != 0) {
        Node* temp = list.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_node = new Node(S, "", temp->level, temp->next);
        temp->next = new_node;
        list.size += 1;
        DNode* head = new DNode(list.head->data, "", list.head->level, nullptr, nullptr);
        temp = list.head->next;
        DNode* tam = head;
        while (temp != nullptr) {
            DNode* new_ = new DNode(temp->data, "", temp->level, tam->next, tam);
            tam->next = new_;
            tam = tam->next;
            temp = temp->next;
        }
        int scope = tam->level;
        tam = tam->prev;
        List list1;
        list1.head = new Node;
        Node* tam1 = list1.head;
        while (tam != nullptr) {
            if (tam->data[0] == 'I') {
                if (tam->level <= scope) {
                    string s;
                    for (int i = 7; i < int(tam->data.length()) && tam->data[i] != ' '; i++) s += tam->data[i];
                    tam1 = list1.head;
                    int mark = 0;
                    while (tam1->next != nullptr) {
                        if (tam1->data == s + "//") {
                            mark += 1;
                            break;
                        }
                        tam1 = tam1->next;
                    }
                    if (tam1->data == s + "//") mark += 1;
                    if (mark == 0) {
                        Node* new_node = new Node(s + "//", "", tam->level, tam1->next);
                        tam1->next = new_node;
                    }
                }
            }
            tam = tam->prev;
        }
        tam1 = list1.head->next;
        if (tam1 != nullptr && tam1->data != "") {
            while (tam1 != nullptr) {
                cout << tam1->data << tam1->level;
                tam1 = tam1->next;
                if (tam1 != nullptr) cout << ' ';
            }
            cout << endl;
        }
        if (line == count_line && new_node->level > 0) {
            UnclosedBlock U(new_node->level);
            cout << U.what();
        }
        while (list1.head != nullptr) {
            Node* tam1 = list1.head;
            list1.head = list1.head->next;
            delete tam1;
            tam1 = nullptr;
        }
        while (head != nullptr) {
            DNode* tam = head;
            head = head->next;
            delete tam;
            tam = nullptr;
        }
    }
}

void SymbolTable::checkPRINT(List& list, string S, int line, int count_line) {
    if (list.size != 0) {
        Node* temp = list.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_node = new Node(S, "", temp->level, temp->next);
        temp->next = new_node;
        list.size += 1;
        DNode* head = new DNode(list.head->data, "", list.head->level, nullptr, nullptr);
        temp = list.head->next;
        DNode* tam = head;
        while (temp != nullptr) {
            DNode* new_ = new DNode(temp->data, "", temp->level, tam->next, tam);
            tam->next = new_;
            tam = tam->next;
            temp = temp->next;
        }
        int scope = tam->level;
        tam = tam->prev;
        List list1;
        list1.head = new Node;
        Node* tam1 = list1.head;
        while (tam != nullptr) {
            if (tam->data[0] == 'I') {
                if (tam->level <= scope) {
                    string s;
                    for (int i = 7; i < int(tam->data.length()) && tam->data[i] != ' '; i++) s += tam->data[i];
                    tam1 = list1.head;
                    int mark = 0;
                    if (tam1->data == s + "//") mark += 1;
                    while (tam1->next != nullptr) {
                        if (tam1->data == s + "//") {
                            mark += 1;
                            break;
                        }
                        tam1 = tam1->next;
                    }
                    if (mark == 0) {
                        Node* new_node = new Node(s + "//", "", tam->level, list1.head);
                        list1.head = new_node;
                    }
                }
            }
            tam = tam->prev;
        }
        tam1 = list1.head;
        if (tam1->data != "") {
            while (tam1->next != nullptr) {
                cout << tam1->data << tam1->level;
                tam1 = tam1->next;
                if (tam1->next != nullptr) cout << ' ';
            }
            cout << endl;
        }
        if (line == count_line && new_node->level > 0) {
            UnclosedBlock U(new_node->level);
            cout << U.what();
        }
        while (list1.head != nullptr) {
            Node* tam1 = list1.head;
            list1.head = list1.head->next;
            delete tam1;
            tam1 = nullptr;
        }
        while (head != nullptr) {
            DNode* tam = head;
            head = head->next;
            delete tam;
            tam = nullptr;
        }
    }
}

string SymbolTable::checkLOOKUP(List& list, string S) {
    if (list.size == 0) {
        list.head = new Node(S, "", 0, nullptr);
        list.size += 1;
    }
    else {
        Node* temp = list.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_ = new Node(S, "", temp->level, temp->next);
        temp->next = new_;
        list.size += 1;
        DNode* head = new DNode(list.head->data, "", list.head->level, nullptr, nullptr);
        DNode* temp1 = head;
        temp = list.head->next;
        while (temp != nullptr) {
            DNode* new_node = new DNode(temp->data, "", temp->level, temp1->next, temp1);
            temp1->next = new_node;
            temp1 = temp1->next;
            temp = temp->next;
        }
        string s;
        int scope = temp1->level;
        for (int i = 7; i < int(temp1->data.length()); i++) s += temp1->data[i];
        temp1 = temp1->prev;
        while (temp1 != nullptr) {
            if (temp1->data[0] == 'I') {
                string s1;
                for (int j = 7; temp1->data[j] != ' '; j++) s1 += temp1->data[j];
                if (s1 == s && temp1->level <= scope) {
                    int n = temp1->level;
                    while (head != nullptr) {
                        DNode* temp1 = head;
                        head = head->next;
                        delete temp1;
                        temp1 = nullptr;
                    }
                    return to_string(n);
                }
            }
            temp1 = temp1->prev;
        }
        while (head != nullptr) {
            DNode* temp1 = head;
            head = head->next;
            delete temp1;
            temp1 = nullptr;
        }
    }
    Undeclared U(S);
    return U.what();
}

void SymbolTable::deleteBLOCK(List& list) {
    Node* temp = list.head;
    while (temp->next != nullptr) temp = temp->next;
    int scope = temp->level + 1;
    temp = list.head;
    if (temp->level != scope) {
        int count = 0;
        while (temp != nullptr && temp->level < scope) {
            count += 1;
            temp = temp->next;
        }
        Node* temp1 = list.head;
        for (int i = 1; i < count; i++) temp1 = temp1->next;
        while (temp1 != nullptr && temp1->next != nullptr) {
            Node* temp2 = temp1->next;
            temp1->next = temp2->next;
            delete temp2;
            temp2 = nullptr;
            list.size -= 1;
        }
    }
    else {
        while (list.head != nullptr) {
            Node* tam = list.head;
            list.head = tam->next;
            delete tam;
            tam = nullptr;
            list.size = 0;
        }
    }
}

void SymbolTable::checkEND(List& list, string S) {
    if (list.size == 0) {
        list.head = new Node(S, "", -1, nullptr);
        list.size += 1;
    }
    else {
        Node* temp = list.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_node = new Node(S, "", temp->level - 1, temp->next);
        temp->next = new_node;
        list.size += 1;
    }
}

void SymbolTable::checkBEGIN(List& list, string S) {
    if (list.size == 0) {
        list.head = new Node(S, "", 1, nullptr);
        list.size += 1;
    }
    else {
        Node* temp = list.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_node = new Node(S, "", temp->level + 1, temp->next);
        temp->next = new_node;
        list.size += 1;
    }
}

string SymbolTable::checkASSIGN(List& list, string S) {
    if (list.size == 0) {
        list.head = new Node(S, "", 0, nullptr);
        list.size += 1;
        Undeclared U(S);
        return U.what();
    }
    else {
        Node* temp = list.head;
        DNode* head = new DNode(temp->data, "", temp->level, nullptr, nullptr), * tmp = head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_ = new Node(S, "", temp->level, temp->next);
        temp->next = new_;
        list.size += 1;
        temp = list.head->next;
        while (temp != nullptr) {
            DNode* new_node = new DNode(temp->data, "", temp->level, tmp->next, tmp);
            tmp->next = new_node;
            tmp = tmp->next;
            temp = temp->next;
        }
        string id, variable;
        int scope = tmp->level, i;
        for (i = 7; i < int(S.length()) && S[i] != ' '; i++) id += S[i];
        for (i = i + 1; i < int(S.length()) && S[i] != ' '; i++) variable += S[i];
        tmp = tmp->prev;
        while (tmp != nullptr) {
            if (tmp->data[0] == 'I' && tmp->level <= scope) {
                if (variable[0] == 39 || (variable[0] >= 48 && variable[0] <= 57)) {
                    int j;
                    string id_tmp;
                    for (j = 7; j < int(tmp->data.length()) && tmp->data[j] != ' '; j++) id_tmp += tmp->data[j];
                    if (id_tmp == id) {
                        string vari_tmp;
                        for (j = j + 1; j < int(tmp->data.length()); j++) vari_tmp += tmp->data[j];
                        if ((variable[0] == 39 && vari_tmp == "string") || (variable[0] >= 48 && variable[0] <= 57 && vari_tmp == "number")) {
                            while (head != nullptr) {
                                DNode* tam = head;
                                head = head->next;
                                delete tam;
                                tam = nullptr;
                            }
                            return "success";
                        }
                        else {
                            while (head != nullptr) {
                                DNode* tam = head;
                                head = head->next;
                                delete tam;
                                tam = nullptr;
                            }
                            TypeMismatch T(S);
                            return T.what();
                        }
                    }
                }
                else {
                    DNode* tmp1 = tmp, * tmp2 = tmp;
                    string id1, id2, vari1, vari2;
                    while (tmp1 != nullptr) {
                        if (tmp1->data[0] == 'I' && tmp1->level <= scope) {
                            int j;
                            for (j = 7; j < int(tmp1->data.length()) && tmp1->data[j] != ' '; j++) id1 += tmp1->data[j];
                            if (id1 == id) {
                                for (j = j + 1; j < int(tmp1->data.length()); j++) vari1 += tmp1->data[j];
                                break;
                            }
                            else id1.clear();
                        }
                        tmp1 = tmp1->prev;
                    }
                    while (tmp2 != nullptr) {
                        if (tmp2->data[0] == 'I' && tmp2->level <= scope) {
                            int j;
                            for (j = 7; j < int(tmp2->data.length()) && tmp2->data[j] != ' '; j++) id2 += tmp2->data[j];
                            if (id2 == variable) {
                                for (j = j + 1; j < int(tmp2->data.length()); j++) vari2 += tmp2->data[j];
                                break;
                            }
                            else id2.clear();
                        }
                        tmp2 = tmp2->prev;
                    }
                    if (vari1 == vari2 && vari1 != "" && vari2 != "") {
                        while (head != nullptr) {
                            DNode* tam = head;
                            head = head->next;
                            delete tam;
                            tam = nullptr;
                        }
                        return "success";
                    }
                    else if (id1 != "" && id2 != "" && vari1 != vari2) {
                        while (head != nullptr) {
                            DNode* tam = head;
                            head = head->next;
                            delete tam;
                            tam = nullptr;
                        }
                        TypeMismatch T(S);
                        return T.what();
                    }
                    else {
                        while (head != nullptr) {
                            DNode* tam = head;
                            head = head->next;
                            delete tam;
                            tam = nullptr;
                        }
                        Undeclared U(S);
                        return U.what();
                    }
                }
            }
            tmp = tmp->prev;
        }
        while (head != nullptr) {
            DNode* tam = head;
            head = head->next;
            delete tam;
            tam = nullptr;
        }
    }
    Undeclared U(S);
    return U.what();
}

string SymbolTable::checkINSERT(List& list, string S) {
    if (list.size == 0) {
        list.head = new Node(S, "", 0, nullptr);
        list.size += 1;
        return  "success";
    }
    else {
        Node* temp = list.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_ = new Node(S, "", temp->level, temp->next);
        int scope = temp->level;
        temp->next = new_;
        list.size += 1;
        temp = list.head;
        string s;
        int mark = 0;
        for (int i = 7; S[i] != ' '; i++) s += S[i];
        for (int i = 1; i < list.size; i++) {
            if (temp->level == scope) {
                string s1;
                for (int j = 0; j < int(temp->data.length()) && temp->data[j] != ' '; j++) s1 += temp->data[j];
                if (s1 == "INSERT") {
                    s1.clear();
                    for (int j = 7; temp->data[j] != ' '; j++) s1 += temp->data[j];
                    if (s1 == s) {
                        mark += 1;
                        break;
                    }
                }
            }
            temp = temp->next;
        }
        if (mark == 1) {
            Redeclared R(S);
            return R.what();
        }
    }
    return "success";
}

bool SymbolTable::checkInstruction(string S) {
    string s;
    int i = 0;
    for (i = 0; i < int(S.length()) && (S[i] >= 65 && S[i] <= 90); i++) s += S[i];
    if (s == "INSERT" || s == "ASSIGN" || s == "BEGIN" || s == "END" || s == "LOOKUP" || s == "PRINT" || s == "RPRINT") {
        if ((s == "BEGIN" || s == "END" || s == "PRINT" || s == "RPRINT") && int(s.length()) == int(S.length())) return true;
        else if (s == "INSERT") {
            int count_mistake = 0;
            s.clear();
            if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' ')) count_mistake += 1;
            i++;
            if (i < int(S.length()) && S[i] >= 97 && S[i] <= 122) {
                for (i = i; i < int(S.length()) && S[i] != ' '; i++) {
                    if ((S[i] < 65 || S[i] > 90) && (S[i] < 97 || S[i] > 122) && (S[i] < 48 || S[i] > 57) && S[i] != '_') count_mistake += 1;
                }
                if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' ')) count_mistake += 1;
                else if (i < int(S.length()) && S[i] == ' ') {
                    for (i = i + 1; i < int(S.length()); i++) {
                        if (S[i] < 97 || S[i] > 122) count_mistake += 1;
                        s += S[i];
                    }
                    if (s != "string" && s != "number") count_mistake += 1;
                }
            }
            else count_mistake += 1;
            if (count_mistake == 0) return true;
        }
        else if (s == "ASSIGN") {
            int count_mistake = 0;
            if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' ')) count_mistake += 1;
            i++;
            if (i < int(S.length()) && (S[i] >= 97 && S[i] <= 122)) {
                for (i = i; i < int(S.length()) && S[i] != ' '; i++) {
                    if ((S[i] < 65 || S[i] > 90) && (S[i] < 97 || S[i] > 122) && (S[i] < 48 || S[i] > 57) && S[i] != '_') count_mistake += 1;
                }
                if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' ')) count_mistake += 1;
                else {
                    i++;
                    if (S[i] == 39 && i < int(S.length()) - 2 && S[int(S.length()) - 1] == 39) {
                        for (i = i + 1; i < int(S.length()) - 1; i++) {
                            if (S[i] != ' ' && (S[i] < 97 || S[i] > 122) && (S[i] < 65 || S[i] > 90) && (S[i] < 48 || S[i] > 57)) count_mistake += 1;
                        }
                    }
                    else if (i < int(S.length()) && S[i] >= 48 && S[i] <= 57) {
                        for (i = i; i < int(S.length()); i++) {
                            if (S[i] < 48 || S[i] > 57) count_mistake += 1;
                        }
                    }
                    else if (i < int(S.length()) && (S[i] >= 97 && S[i] <= 122)) {
                        for (i = i; i < int(S.length()); i++) {
                            if (S[i] == '_' || (S[i] >= 48 && S[i] <= 57) || (S[i] >= 65 && S[i] <= 90) || (S[i] >= 97 && S[i] <= 122)) count_mistake += 0;
                            else count_mistake += 1;
                        }
                    }
                    else if (S[i] == 39 && S[++i] == 39) count_mistake += 0;
                    else count_mistake += 1;
                }
            }
            else count_mistake += 1;
            if (count_mistake == 0) return true;
        }
        else if (s == "LOOKUP") {
            int count_mistake = 0;
            if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' ')) count_mistake += 1;
            i++;
            if (i < int(S.length()) && S[i] >= 97 && S[i] <= 122) {
                for (i = i; i < int(S.length()); i++) {
                    if ((S[i] < 65 || S[i] > 90) && (S[i] < 97 || S[i] > 122) && (S[i] < 48 || S[i] > 57) && S[i] != '_') count_mistake += 1;
                }
            }
            else count_mistake += 1;
            if (count_mistake == 0) return true;
        }
    }
    return false;
}

void SymbolTable::run(string filename) {
    fstream new_file, file;
    new_file.open(filename, ios::in);
    file.open(filename, ios::in);
    int count_line = 0;
    if (file.is_open()) {
        string S1;
        while (getline(file, S1)) count_line += 1;
    }
    file.close();
    if (new_file.is_open()) {
        string S;
        List list;
        int line = 0;
        while (getline(new_file, S)) {
            line += 1;
            bool temp = checkInstruction(S);
            if (temp == false) {
                InvalidInstruction I(S);
                cout << I.what();
                break;
            }
            string s;
            for (int i = 0; i < int(S.length()) && S[i] != ' '; i++) s += S[i];
            if (s == "INSERT") {
                string T = checkINSERT(list, S);
                Node* tam = list.head;
                while (tam->next != nullptr) tam = tam->next;
                if (line == count_line) {
                    if (T == "success" && tam->level == 0) cout << "success" << endl;
                    else if (T == "success" && tam->level > 0) {
                        cout << "success" << endl;
                        UnclosedBlock U(tam->level);
                        cout << U.what();
                    }
                    else if (T != "success") cout << T;
                }
                else {
                    if (T == "success") cout << T << endl;
                    else {
                        cout << T;
                        break;
                    }
                }
            }
            else if (s == "ASSIGN") {
                string T = checkASSIGN(list, S);
                Node* tam = list.head;
                while (tam->next != nullptr) tam = tam->next;
                if (line == count_line) {
                    if (tam->level == 0) {
                        if (T == "success") cout << T << endl;
                        else cout << T;
                    }
                    else {
                        if (T == "success") {
                            cout << T << endl;
                            UnclosedBlock U(tam->level);
                            cout << U.what();
                        }
                        else cout << T;
                    }
                }
                else {
                    if (T == "success") cout << T << endl;
                    else {
                        cout << T;
                        break;
                    }
                }
            }
            else if (s == "BEGIN") {
                checkBEGIN(list, S);
                Node* tam = list.head;
                while (tam->next != nullptr) tam = tam->next;
                if (line == count_line && tam->level > 0) {
                    UnclosedBlock U(tam->level);
                    cout << U.what();
                }
            }
            else if (s == "END") {
                checkEND(list, S);
                Node* tam = list.head;
                while (tam->next != nullptr) tam = tam->next;
                if (line == count_line) {
                    if (tam->level < 0) {
                        UnknownBlock U;
                        cout << U.what();
                    }
                    else if (tam->level > 0) {
                        UnclosedBlock U(tam->level);
                        cout << U.what();
                    }
                }
                else if (line != count_line && tam->level < 0) {
                    UnknownBlock U;
                    cout << U.what();
                    break;
                }
                else deleteBLOCK(list);
            }
            else if (s == "LOOKUP") {
                string T = checkLOOKUP(list, S);
                Node* tam = list.head;
                while (tam->next != nullptr) tam = tam->next;
                if (line == count_line && tam->level > 0) {
                    if (T[0] >= 48 && T[0] <= 57) {
                        cout << T << endl;
                        UnclosedBlock U(tam->level);
                        cout << U.what();
                    }
                    else cout << T;
                }
                else {
                    if (T[0] >= 48 && T[0] <= 57) cout << T << endl;
                    else {
                        cout << T;
                        break;
                    }
                }
            }
            else if (s == "PRINT") checkPRINT(list, S, line, count_line);
            else if (s == "RPRINT") checkRPRINT(list, S, line, count_line);
        }
        while (list.head != nullptr) {
            Node* tmp = list.head;
            list.head = list.head->next;
            delete tmp;
            tmp = nullptr;
            list.size = 0;
        }
    }
    new_file.close();
}