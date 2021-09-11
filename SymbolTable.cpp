#include "SymbolTable.h"

void SymbolTable::checkRPRINT(Ds& ds, string S, int line, int count_line) {
    if (ds.size != 0) {
        Node* temp = ds.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_node = new Node(S, "", temp->level, temp->next);
        temp->next = new_node;
        ds.size += 1;
        DDs dds;
        dds.head = new DNode(ds.head->data, "", ds.head->level, nullptr, nullptr);
        temp = ds.head->next;
        DNode* tam = dds.head;
        while (temp != nullptr) {
            DNode* new_ = new DNode(temp->data, "", temp->level, tam->next, tam);
            tam->next = new_;
            tam = tam->next;
            temp = temp->next;
        }
        int scope = tam->level;
        tam = tam->prev;
        Ds ds1;
        ds1.head = new Node;
        Node* tam1 = ds1.head;
        while (tam != nullptr) {
            if (tam->data[0] == 'I') {
                if (tam->level <= scope) {
                    string s;
                    for (int i = 7; i < int(tam->data.length()) && tam->data[i] != ' '; i++) s += tam->data[i];
                    tam1 = ds1.head;
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
        tam1 = ds1.head->next;
        if (tam1 != nullptr && tam1->data != "") {
            while (tam1 != nullptr) {
                cout << tam1->data << tam1->level;
                tam1 = tam1->next;
                if (tam1 != nullptr) cout << ' ';
            }
            cout << endl;
        }
        deleteDs(ds1);
        deleteDDs(dds);
        if (line == count_line && new_node->level > 0) {
            int level = new_node->level;
            deleteDs(ds);
            throw UnclosedBlock(level);
        }
    }
}

void SymbolTable::checkPRINT(Ds& ds, string S, int line, int count_line) {
    if (ds.size != 0) {
        Node* temp = ds.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_node = new Node(S, "", temp->level, temp->next);
        temp->next = new_node;
        ds.size += 1;
        DDs dds;
        dds.head = new DNode(ds.head->data, "", ds.head->level, nullptr, nullptr);
        temp = ds.head->next;
        DNode* tam = dds.head;
        while (temp != nullptr) {
            DNode* new_ = new DNode(temp->data, "", temp->level, tam->next, tam);
            tam->next = new_;
            tam = tam->next;
            temp = temp->next;
        }
        int scope = tam->level;
        tam = tam->prev;
        Ds ds1;
        ds1.head = new Node;
        Node* tam1 = ds1.head;
        while (tam != nullptr) {
            if (tam->data[0] == 'I') {
                if (tam->level <= scope) {
                    string s;
                    for (int i = 7; i < int(tam->data.length()) && tam->data[i] != ' '; i++) s += tam->data[i];
                    tam1 = ds1.head;
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
                        Node* new_node = new Node(s + "//", "", tam->level, ds1.head);
                        ds1.head = new_node;
                    }
                }
            }
            tam = tam->prev;
        }
        tam1 = ds1.head;
        if (tam1->data != "") {
            while (tam1->next != nullptr) {
                cout << tam1->data << tam1->level;
                tam1 = tam1->next;
                if (tam1->next != nullptr) cout << ' ';
            }
            cout << endl;
        }
        deleteDs(ds1);
        deleteDDs(dds);
        if (line == count_line && new_node->level > 0) {
            int level = new_node->level;
            deleteDs(ds);
            throw UnclosedBlock(level);
        }
    }
}

string SymbolTable::checkLOOKUP(Ds& ds, string S) {
    if (ds.size == 0) {
        ds.head = new Node(S, "", 0, nullptr);
        ds.size += 1;
    }
    else {
        Node* temp = ds.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_ = new Node(S, "", temp->level, temp->next);
        temp->next = new_;
        ds.size += 1;
        DDs dds;
        dds.head = new DNode(ds.head->data, "", ds.head->level, nullptr, nullptr);
        DNode* temp1 = dds.head;
        temp = ds.head->next;
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
                    deleteDDs(dds);
                    return to_string(n);
                }
            }
            temp1 = temp1->prev;
        }
        deleteDDs(dds);
    }
    Undeclared U(S);
    return U.what();
}

void SymbolTable::deleteBLOCK(Ds& ds) {
    Node* temp = ds.head;
    while (temp->next != nullptr) temp = temp->next;
    int scope = temp->level + 1;
    temp = ds.head;
    if (temp->level != scope) {
        int count = 0;
        while (temp != nullptr && temp->level < scope) {
            count += 1;
            temp = temp->next;
        }
        Node* temp1 = ds.head;
        for (int i = 1; i < count; i++) temp1 = temp1->next;
        while (temp1 != nullptr && temp1->next != nullptr) {
            Node* temp2 = temp1->next;
            temp1->next = temp2->next;
            delete temp2;
            temp2 = nullptr;
            ds.size -= 1;
        }
    }
    else deleteDs(ds);
}

void SymbolTable::checkEND(Ds& ds, string S) {
    if (ds.size == 0) {
        ds.head = new Node(S, "", -1, nullptr);
        ds.size += 1;
    }
    else {
        Node* temp = ds.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_node = new Node(S, "", temp->level - 1, temp->next);
        temp->next = new_node;
        ds.size += 1;
    }
}

void SymbolTable::checkBEGIN(Ds& ds, string S) {
    if (ds.size == 0) {
        ds.head = new Node(S, "", 1, nullptr);
        ds.size += 1;
    }
    else {
        Node* temp = ds.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_node = new Node(S, "", temp->level + 1, temp->next);
        temp->next = new_node;
        ds.size += 1;
    }
}

string SymbolTable::checkASSIGN(Ds& ds, string S) {
    if (ds.size == 0) {
        ds.head = new Node(S, "", 0, nullptr);
        ds.size += 1;
        Undeclared U(S);
        return U.what();
    }
    else {
        Node* temp = ds.head;
        DDs dds;
        dds.head = new DNode(temp->data, "", temp->level, nullptr, nullptr);
        DNode* tmp = dds.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_ = new Node(S, "", temp->level, temp->next);
        temp->next = new_;
        ds.size += 1;
        temp = ds.head->next;
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
                            deleteDDs(dds);
                            return "success";
                        }
                        else {
                            deleteDDs(dds);
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
                        deleteDDs(dds);
                        return "success";
                    }
                    else if (id1 != "" && id2 != "" && vari1 != vari2) {
                        deleteDDs(dds);
                        TypeMismatch T(S);
                        return T.what();
                    }
                    else {
                        deleteDDs(dds);
                        Undeclared U(S);
                        return U.what();
                    }
                }
            }
            tmp = tmp->prev;
        }
        deleteDDs(dds);
    }
    Undeclared U(S);
    return U.what();
}

void SymbolTable::checkINSERT(Ds& ds, string S, int line, int count_line) {
    if (ds.size == 0) {
        ds.head = new Node(S, "", 0, nullptr);
        ds.size += 1;
        cout << "success" << endl;
    }
    else {
        Node* temp = ds.head;
        while (temp->next != nullptr) temp = temp->next;
        Node* new_ = new Node(S, "", temp->level, temp->next);
        int scope = temp->level;
        temp->next = new_;
        ds.size += 1;
        temp = ds.head;
        string s;
        int mark = 0;
        for (int i = 7; S[i] != ' '; i++) s += S[i];
        for (int i = 1; i < ds.size; i++) {
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
            deleteDs(ds);
            throw Redeclared(S);
        }
        else {
            if (line == count_line && new_->level > 0) {
                int level = new_->level;
                cout << "success" << endl;
                deleteDs(ds);
                throw UnclosedBlock(level);
            }
            else cout << "success" << endl;
        }
    }
}

void SymbolTable::checkInstruction(Ds& ds, string S) {
    string s;
    int i = 0;
    for (i = 0; i < int(S.length()) && (S[i] >= 65 && S[i] <= 90); i++) s += S[i];
    if (s == "INSERT" || s == "ASSIGN" || s == "BEGIN" || s == "END" || s == "LOOKUP" || s == "PRINT" || s == "RPRINT") {
        int count_mistake = 0;
        if ((s == "BEGIN" || s == "END" || s == "PRINT" || s == "RPRINT") && int(s.length()) == int(S.length()));
        else if (s == "INSERT" || s == "LOOKUP" || s == "ASSIGN") {
            if (s == "INSERT") {
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
            }
            else if (s == "ASSIGN") {
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
                        else if (S[i] == 39 && S[i + 1] == 39) count_mistake += 0;
                        else count_mistake += 1;
                    }
                }
                else count_mistake += 1;
            }
            else if (s == "LOOKUP") {
                if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' ')) count_mistake += 1;
                i++;
                if (i < int(S.length()) && S[i] >= 97 && S[i] <= 122) {
                    for (i = i; i < int(S.length()); i++) {
                        if ((S[i] < 65 || S[i] > 90) && (S[i] < 97 || S[i] > 122) && (S[i] < 48 || S[i] > 57) && S[i] != '_') count_mistake += 1;
                    }
                }
                else count_mistake += 1;
            }
        }
        else count_mistake += 1;
        if (count_mistake != 0) {
            deleteDs(ds);
            throw InvalidInstruction(S);
        }
    }
    else {
        deleteDs(ds);
        throw InvalidInstruction(S);
    }
}

void SymbolTable::deleteDs(Ds& ds) {
    while (ds.head != nullptr) {
        Node* tmp = ds.head;
        ds.head = ds.head->next;
        delete tmp;
        tmp = nullptr;
        ds.size = 0;
    }
}

void SymbolTable::deleteDDs(DDs& dds) {
    while (dds.head != nullptr) {
        DNode* tmp = dds.head;
        dds.head = dds.head->next;
        delete tmp;
        tmp = nullptr;
    }
}

void SymbolTable::run(string filename) {
    fstream file(filename);
    int count_line = 0;
    if (file) {
        string S1;
        while (getline(file, S1)) count_line += 1;
    }
    file.close();
    fstream new_file(filename);
    if (new_file) {
        string S;
        Ds ds;
        int line = 0;
        while (getline(new_file, S)) {
            line += 1;
            checkInstruction(ds, S);
            string s;
            for (int i = 0; i < int(S.length()) && S[i] != ' '; i++) s += S[i];
            if (s == "INSERT") checkINSERT(ds, S, line, count_line);
            else if (s == "ASSIGN") {
                string T = checkASSIGN(ds, S);
                Node* tam = ds.head;
                while (tam->next != nullptr) tam = tam->next;
                if (line == count_line) {
                    if (tam->level == 0) {
                        if (T == "success") {
                            deleteDs(ds);
                            cout << T << endl;
                        }
                        else if (T[0] == 'U') {
                            deleteDs(ds);
                            throw Undeclared(S);
                        }
                        else if (T[0] == 'T') {
                            deleteDs(ds);
                            throw TypeMismatch(S);
                        }
                    }
                    else {
                        if (T == "success") {
                            cout << T << endl;
                            int n = tam->level;
                            deleteDs(ds);
                            throw UnclosedBlock(n);
                        }
                        else if (T[0] == 'U') {
                            deleteDs(ds);
                            throw Undeclared(S);
                        }
                        else if (T[0] == 'T') {
                            deleteDs(ds);
                            throw TypeMismatch(S);
                        }
                    }
                }
                else {
                    if (T == "success") cout << T << endl;
                    else if (T[0] == 'U') {
                        deleteDs(ds);
                        throw Undeclared(S);
                    }
                    else if (T[0] == 'T') {
                        deleteDs(ds);
                        throw TypeMismatch(S);
                    }
                }
            }
            else if (s == "BEGIN") {
                checkBEGIN(ds, S);
                Node* tam = ds.head;
                while (tam->next != nullptr) tam = tam->next;
                if (line == count_line && tam->level > 0) {
                    int level = tam->level;
                    deleteDs(ds);
                    throw UnclosedBlock(level);
                }
            }
            else if (s == "END") {
                checkEND(ds, S);
                Node* tam = ds.head;
                while (tam->next != nullptr) tam = tam->next;
                if (line == count_line) {
                    if (tam->level < 0) {
                        int level = tam->level;
                        deleteDs(ds);
                        throw UnknownBlock();
                    }
                    else if (tam->level > 0) {
                        int level = tam->level;
                        deleteDs(ds);
                        throw UnclosedBlock(level);
                    }
                }
                else if (line != count_line && tam->level < 0) {
                    int level = tam->level;
                    deleteDs(ds);
                    throw UnknownBlock();
                }
                else deleteBLOCK(ds);
            }
            else if (s == "LOOKUP") {
                string T = checkLOOKUP(ds, S);
                Node* tam = ds.head;
                while (tam->next != nullptr) tam = tam->next;
                if (line == count_line && tam->level > 0) {
                    if (T[0] >= 48 && T[0] <= 57) {
                        cout << T << endl;
                        int level = tam->level;
                        deleteDs(ds);
                        throw UnclosedBlock(level);
                    }
                    else {
                        deleteDs(ds);
                        throw Undeclared(S);
                    }
                }
                else {
                    if (T[0] >= 48 && T[0] <= 57) cout << T << endl;
                    else {
                        deleteDs(ds);
                        throw Undeclared(S);
                    }
                }
            }
            else if (s == "PRINT") checkPRINT(ds, S, line, count_line);
            else if (s == "RPRINT") checkRPRINT(ds, S, line, count_line);
        }
        deleteDs(ds);
    }
}