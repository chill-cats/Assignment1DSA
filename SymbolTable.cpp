#include "SymbolTable.h"
#include "error.h"

void SymbolTable::checkRPRINT(List &list, string S, int line, int count_line) {
  if (list.size != 0) {
    Node *temp = list.head;
    while (temp->next != nullptr)
      temp = temp->next;
    Node *new_node = new Node(S, "", temp->level, temp->next);
    temp->next = new_node;
    list.size += 1;
    DNode *head =
        new DNode(list.head->data, "", list.head->level, nullptr, nullptr);
    temp = list.head->next;
    DNode *tam = head;
    while (temp != nullptr) {
      DNode *new_ = new DNode(temp->data, "", temp->level, tam->next, tam);
      tam->next = new_;
      tam = tam->next;
      temp = temp->next;
    }
    int scope = tam->level;
    tam = tam->prev;
    List list1;
    list1.head = new Node;
    Node *tam1 = list1.head;
    while (tam != nullptr) {
      if (tam->data[0] == 'I') {
        if (tam->level <= scope) {
          string s;
          for (int i = 7; i < int(tam->data.length()) && tam->data[i] != ' ';
               i++)
            s += tam->data[i];
          tam1 = list1.head;
          int mark = 0;
          while (tam1->next != nullptr) {
            if (tam1->data == s + "//") {
              mark += 1;
              break;
            }
            tam1 = tam1->next;
          }
          if (tam1->data == s + "//")
            mark += 1;
          if (mark == 0) {
            Node *new_node = new Node(s + "//", "", tam->level, tam1->next);
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
        if (tam1 != nullptr)
          cout << ' ';
      }
      cout << endl;
    }
    if (line == count_line) {
      if (new_node->level > 0) {
        UnclosedBlock U(new_node->level);
        cout << U.what();
      }
      while (list.head != nullptr) {
        Node *tam_ = list.head;
        list.head = list.head->next;
        delete tam_;
        tam_ = nullptr;
        list.size = 0;
      }
    }
    while (list1.head != nullptr) {
      Node *tam1 = list1.head;
      list1.head = list1.head->next;
      delete tam1;
      tam1 = nullptr;
    }
    while (head != nullptr) {
      DNode *tam = head;
      head = head->next;
      delete tam;
      tam = nullptr;
    }
  }
}

void SymbolTable::checkPRINT(List &list, string S, int line, int count_line) {
  if (list.size != 0) {
    Node *temp = list.head;
    while (temp->next != nullptr)
      temp = temp->next;
    Node *new_node = new Node(S, "", temp->level, temp->next);
    temp->next = new_node;
    list.size += 1;
    DNode *head =
        new DNode(list.head->data, "", list.head->level, nullptr, nullptr);
    temp = list.head->next;
    DNode *tam = head;
    while (temp != nullptr) {
      DNode *new_ = new DNode(temp->data, "", temp->level, tam->next, tam);
      tam->next = new_;
      tam = tam->next;
      temp = temp->next;
    }
    int scope = tam->level;
    tam = tam->prev;
    List list1;
    list1.head = new Node;
    Node *tam1 = list1.head;
    while (tam != nullptr) {
      if (tam->data[0] == 'I') {
        if (tam->level <= scope) {
          string s;
          for (int i = 7; i < int(tam->data.length()) && tam->data[i] != ' ';
               i++)
            s += tam->data[i];
          tam1 = list1.head;
          int mark = 0;
          if (tam1->data == s + "//")
            mark += 1;
          while (tam1->next != nullptr) {
            if (tam1->data == s + "//") {
              mark += 1;
              break;
            }
            tam1 = tam1->next;
          }
          if (mark == 0) {
            Node *new_node = new Node(s + "//", "", tam->level, list1.head);
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
        if (tam1->next != nullptr)
          cout << ' ';
      }
      cout << endl;
    }
    if (line == count_line) {
      if (new_node->level > 0) {
        UnclosedBlock U(new_node->level);
        cout << U.what();
      }
      while (list.head != nullptr) {
        Node *tam_ = list.head;
        list.head = list.head->next;
        delete tam_;
        tam_ = nullptr;
        list.size = 0;
      }
    }
    while (list1.head != nullptr) {
      Node *tam1 = list1.head;
      list1.head = list1.head->next;
      delete tam1;
      tam1 = nullptr;
    }
    while (head != nullptr) {
      DNode *tam = head;
      head = head->next;
      delete tam;
      tam = nullptr;
    }
  }
}

string SymbolTable::checkLOOKUP(List &list, string S) {
  if (list.size == 0) {
    list.head = new Node(S, "", 0, nullptr);
    list.size += 1;
  } else {
    Node *temp = list.head;
    while (temp->next != nullptr)
      temp = temp->next;
    Node *new_ = new Node(S, "", temp->level, temp->next);
    temp->next = new_;
    list.size += 1;
    DNode *head =
        new DNode(list.head->data, "", list.head->level, nullptr, nullptr);
    DNode *temp1 = head;
    temp = list.head->next;
    while (temp != nullptr) {
      DNode *new_node =
          new DNode(temp->data, "", temp->level, temp1->next, temp1);
      temp1->next = new_node;
      temp1 = temp1->next;
      temp = temp->next;
    }
    string s;
    int scope = temp1->level;
    for (int i = 7; i < int(temp1->data.length()); i++)
      s += temp1->data[i];
    temp1 = temp1->prev;
    while (temp1 != nullptr) {
      if (temp1->data[0] == 'I') {
        string s1;
        for (int j = 7; temp1->data[j] != ' '; j++)
          s1 += temp1->data[j];
        if (s1 == s && temp1->level <= scope)
          return to_string(temp1->level);
      }
      temp1 = temp1->prev;
    }
    while (head != nullptr) {
      DNode *temp1 = head;
      head = head->next;
      delete temp1;
      temp1 = nullptr;
    }
  }
  Undeclared U(S);
  return U.what();
}

void SymbolTable::deleteBLOCK(List &list) {
  Node *temp = list.head;
  while (temp->next != nullptr)
    temp = temp->next;
  int scope = temp->level + 1;
  temp = list.head;
  if (temp->level != scope) {
    int count = 0;
    while (temp != nullptr && temp->level < scope) {
      count += 1;
      temp = temp->next;
    }
    Node *temp1 = list.head;
    for (int i = 1; i < count; i++)
      temp1 = temp1->next;
    while (temp1 != nullptr && temp1->next != nullptr) {
      Node *temp2 = temp1->next;
      temp1->next = temp2->next;
      delete temp2;
      temp2 = nullptr;
      list.size -= 1;
    }
  } else {
    while (list.head != nullptr) {
      Node *tam = list.head;
      list.head = tam->next;
      delete tam;
      tam = nullptr;
      list.size = 0;
    }
  }
}

void SymbolTable::checkEND(List &list, string S) {
  if (list.size == 0) {
    list.head = new Node(S, "", -1, nullptr);
    list.size += 1;
  } else {
    Node *temp = list.head;
    while (temp->next != nullptr)
      temp = temp->next;
    Node *new_node = new Node(S, "", temp->level - 1, temp->next);
    temp->next = new_node;
    list.size += 1;
  }
}

void SymbolTable::checkBEGIN(List &list, string S) {
  if (list.size == 0) {
    list.head = new Node(S, "", 1, nullptr);
    list.size += 1;
  } else {
    Node *temp = list.head;
    while (temp->next != nullptr)
      temp = temp->next;
    Node *new_node = new Node(S, "", temp->level + 1, temp->next);
    temp->next = new_node;
    list.size += 1;
  }
}

string SymbolTable::checkASSIGN(List &list, string S) {
  if (list.size == 0) {
    list.head = new Node(S, "", 0, nullptr);
    list.size += 1;
    Undeclared U(S);
    return U.what();
  } else {
    Node *temp = list.head;
    while (temp->next != nullptr)
      temp = temp->next;
    Node *new_ = new Node(S, "", temp->level, temp->next);
    temp->next = new_;
    list.size += 1;
    temp = list.head;
    int count_whitespace = 0, i = 0;
    string s;
    for (int j = 7; S[j] != ' '; j++)
      s += S[j];
    for (i = 0; count_whitespace != 2; i++) {
      if (S[i] == ' ')
        count_whitespace += 1;
    }
    if (S[i] == 39) {
      int mark = 0;
      for (int count = 1; count < list.size; count++) {
        string s1;
        int j = 0;

        for (j = 0; j < int(temp->data.length()) && temp->data[j] != ' '; j++) {
          s1 += temp->data[j];
        }

        s1 += temp->data[j];
        for (j = j + 1; j < int(temp->data.length()) && temp->data[j] != ' ';
             j++)
          s1 += temp->data[j];
        if ("INSERT " + s == s1) {
          mark += 1;
          s1 += temp->data[j];
          for (j = j + 1; j < int(temp->data.length()); j++)
            s1 += temp->data[j];
          if ("INSERT " + s + " string" == s1)
            mark += 1;
          break;
        }
        temp = temp->next;
      }
      if (mark == 0) {
        Undeclared U(S);
        return U.what();
      } else if (mark == 1) {
        TypeMismatch T(S);
        return T.what();
      }
    } else if (S[i] >= 48 && S[i] <= 57) {
      int mark = 0;
      for (int count = 1; count < list.size; count++) {
        string s1;
        int j = 0;
        for (j = 0; j < int(temp->data.length()) && temp->data[j] != ' '; j++)
          s1 += temp->data[j];
        s1 += temp->data[j];
        for (j = j + 1; j < int(temp->data.length()) && temp->data[j] != ' ';
             j++)
          s1 += temp->data[j];
        if ("INSERT " + s == s1) {
          mark += 1;
          s1 += temp->data[j];
          for (j = j + 1; j < int(temp->data.length()); j++)
            s1 += temp->data[j];
          if ("INSERT " + s + " number" == s1)
            mark += 1;
          break;
        }
        temp = temp->next;
      }
      if (mark == 0) {
        Undeclared U(S);
        return U.what();
      } else if (mark == 1) {
        TypeMismatch T(S);
        return T.what();
      }
    } else if (S[i] >= 97 && S[i] <= 122) {
      int mark = 0;
      for (int count = 1; count < list.size; count++) {
        string s1;
        int j = 0;
        for (j = 0; j < int(temp->data.length()) && temp->data[j] != ' '; j++)
          s1 += temp->data[j];
        s1 += temp->data[j];
        for (j = j + 1; j < int(temp->data.length()) && temp->data[j] != ' ';
             j++)
          s1 += temp->data[j];
        if (s1 == "INSERT " + s) {
          string s2;
          for (i = i; i < int(S.length()); i++)
            s2 += S[i];
          Node *temp1 = list.head;
          for (int count_ = 1; count_ < list.size; count_++) {
            string s3;
            int k = 0;
            for (k = 0; k < int(temp1->data.length()) && temp1->data[k] != ' ';
                 k++)
              s3 += temp1->data[k];
            s3 += temp->data[k];
            for (k = k + 1;
                 k < int(temp1->data.length()) && temp1->data[k] != ' '; k++)
              s3 += temp1->data[k];
            if (s3 == "INSERT " + s2) {
              mark += 1;
              string s4, s5;
              for (j = j + 1; j < int(temp->data.length()); j++)
                s4 += temp->data[j];
              for (k = k + 1; k < int(temp1->data.length()); k++)
                s5 += temp1->data[k];
              if (s4 == s5)
                mark += 1;
              break;
            }
            temp1 = temp1->next;
          }
          break;
        }
        temp = temp->next;
      }
      if (mark == 0) {
        Undeclared U(S);
        return U.what();
      } else if (mark == 1) {
        TypeMismatch T(S);
        return T.what();
      }
    }
  }
  return "success";
}

void SymbolTable::checkINSERT(List &list, string S) {
  if (list.size == 0) {
    list.head = new Node(S, "", 0, nullptr);
    list.size += 1;
    return;
  }

  else {
    Node *temp = list.head;
    while (temp->next != nullptr) {
      temp = temp->next; // chay toi tail cua list
    }

    Node *new_ = new Node(
        S, "", temp->level,
        nullptr); // tao node moi voi cai data la nguyen cai line cua file input

    temp->next = new_; // insert vao tail cua list
    list.size += 1;

    string s;
    for (int i = 7; S[i] != ' '; i++) {
      s += S[i];
    }
    // s = name  cua identifier

    temp = list.head;
    int scope = temp->level;

    for (int i = 1; i < list.size; i++) {
      if (temp->level == scope) {
        string s1;

        for (int j = 0; j < int(temp->data.length()) && temp->data[j] != ' ';
             j++) {
          s1 += temp->data[j];
        }

        if (s1 == "INSERT") {
          s1.clear();
          for (int j = 7; temp->data[j] != ' '; j++)
            s1 += temp->data[j];
          if (s1 == s) {
            throw Redeclared(S);
          }
        }
      }
      temp = temp->next;
    }
  }
}

bool SymbolTable::checkInstruction(string S) {
  string s;
  int i = 0;
  for (i = 0; i < int(S.length()) && (S[i] >= 65 && S[i] <= 90); i++)
    s += S[i];
  if (s == "INSERT" || s == "ASSIGN" || s == "BEGIN" || s == "END" ||
      s == "LOOKUP" || s == "PRINT" || s == "RPRINT") {
    if ((s == "BEGIN" || s == "END" || s == "PRINT" || s == "RPRINT") &&
        int(s.length()) == int(S.length()))
      return true;
    else if (s == "INSERT") {
      int count_mistake = 0;
      s.clear();
      if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' '))
        count_mistake += 1;
      i++;
      if (i < int(S.length()) && S[i] >= 97 && S[i] <= 122) {
        for (i = i; i < int(S.length()) && S[i] != ' '; i++) {
          if ((S[i] < 65 || S[i] > 90) && (S[i] < 97 || S[i] > 122) &&
              (S[i] < 48 || S[i] > 57) && S[i] != '_')
            count_mistake += 1;
        }
        if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' '))
          count_mistake += 1;
        else if (i < int(S.length()) && S[i] == ' ') {
          for (i = i + 1; i < int(S.length()); i++) {
            if (S[i] < 97 || S[i] > 122)
              count_mistake += 1;
            s += S[i];
          }
          if (s != "string" && s != "number")
            count_mistake += 1;
        }
      } else
        count_mistake += 1;
      if (count_mistake == 0)
        return true;
    } else if (s == "ASSIGN") {
      int count_mistake = 0;
      if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' '))
        count_mistake += 1;
      i++;
      if (i < int(S.length()) && (S[i] >= 97 && S[i] <= 122)) {
        for (i = i; i < int(S.length()) && S[i] != ' '; i++) {
          if ((S[i] < 65 || S[i] > 90) && (S[i] < 97 || S[i] > 122) &&
              (S[i] < 48 || S[i] > 57) && S[i] != '_')
            count_mistake += 1;
        }
        if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' '))
          count_mistake += 1;
        else {
          i++;
          if (S[i] == 39 && i < int(S.length()) - 2 &&
              S[int(S.length()) - 1] == 39) {
            for (i = i + 1; i < int(S.length()) - 1; i++) {
              if (S[i] != ' ' && (S[i] < 97 || S[i] > 122) &&
                  (S[i] < 65 || S[i] > 90) && (S[i] < 48 || S[i] > 57))
                count_mistake += 1;
            }
          } else if (i < int(S.length()) && S[i] >= 48 && S[i] <= 57) {
            for (i = i; i < int(S.length()); i++) {
              if (S[i] < 48 || S[i] > 57)
                count_mistake += 1;
            }
          } else if (i < int(S.length()) && (S[i] >= 97 && S[i] <= 122)) {
            for (i = i; i < int(S.length()); i++) {
              if (S[i] == '_' || (S[i] >= 48 && S[i] <= 57) ||
                  (S[i] >= 65 && S[i] <= 90) || (S[i] >= 97 && S[i] <= 122))
                count_mistake += 0;
              else
                count_mistake += 1;
            }
          } else if (S[i] == 39 && S[++i] == 39)
            count_mistake += 0;
          else
            count_mistake += 1;
        }
      } else
        count_mistake += 1;
      if (count_mistake == 0)
        return true;
    } else if (s == "LOOKUP") {
      int count_mistake = 0;
      if (i == int(S.length()) || (i < int(S.length()) && S[i] != ' '))
        count_mistake += 1;
      i++;
      if (i < int(S.length()) && S[i] >= 97 && S[i] <= 122) {
        for (i = i; i < int(S.length()); i++) {
          if ((S[i] < 65 || S[i] > 90) && (S[i] < 97 || S[i] > 122) &&
              (S[i] < 48 || S[i] > 57) && S[i] != '_')
            count_mistake += 1;
        }
      } else
        count_mistake += 1;
      if (count_mistake == 0)
        return true;
    }
  }
  return false;
}

void SymbolTable::run(string filename) {
  fstream new_file(filename);
  fstream file(filename);

  int count_line = 0;

  if (file.is_open()) { // đọc xem file có bao nhiêu line
    string S1;
    while (getline(file, S1)) {
      count_line += 1;
    }
  }
  file.close();

  if (new_file.is_open()) {
    string S;
    List list;
    int line = 0;

    while (getline(new_file, S)) {
      line += 1;
      bool isValidInstruction = checkInstruction(S);

      if (isValidInstruction == false) {
        InvalidInstruction exception(S);
        cout << exception.what();
        break;
      }

      string s; // S.length = size_t tim cai token dau tien
      for (int i = 0; i < int(S.length()) && S[i] != ' '; i++) {
        s += S[i];
      }

      if (s == "INSERT") {
        checkINSERT(list, S);
        cout << "success" << endl;

        Node *tam = list.head;

        while (tam->next != nullptr) {
          // chay toi cai tail cua list
          tam = tam->next;
        }

        if (line == count_line) {
          if (tam->level > 0) {
            cout << "success" << endl;
            throw UnclosedBlock(tam->level);
          }
        } else {
          cout << "success" << endl;
        }

      } else if (s == "ASSIGN") {
        string T = checkASSIGN(list, S);
        Node *tam = list.head;
        while (tam->next != nullptr)
          tam = tam->next;
        if (line == count_line) {
          if (tam->level == 0) {
            if (T == "success")
              cout << T << endl;
            else
              cout << T;
          } else {
            if (T == "success") {
              cout << T << endl;
              UnclosedBlock U(tam->level);
              cout << U.what();
            } else
              cout << T;
          }
        } else {
          if (T == "success")
            cout << T << endl;
          else {
            cout << T;
            break;
          }
        }
      } else if (s == "BEGIN") {
        checkBEGIN(list, S);
        Node *tam = list.head;
        while (tam->next != nullptr)
          tam = tam->next;
        if (line == count_line && tam->level > 0) {
          UnclosedBlock U(tam->level);
          cout << U.what();
        }
      } else if (s == "END") {
        checkEND(list, S);
        Node *tam = list.head;
        while (tam->next != nullptr)
          tam = tam->next;
        if (line == count_line) {
          if (tam->level < 0) {
            UnknownBlock U;
            cout << U.what();
          } else if (tam->level > 0) {
            UnclosedBlock U(tam->level);
            cout << U.what();
          }
        } else if (line != count_line && tam->level < 0) {
          UnknownBlock U;
          cout << U.what();
          break;
        } else
          deleteBLOCK(list);
      } else if (s == "LOOKUP") {
        string T = checkLOOKUP(list, S);
        Node *tam = list.head;
        while (tam->next != nullptr)
          tam = tam->next;
        if (line == count_line && tam->level > 0) {
          if (T[0] >= 48 && T[0] <= 57) {
            cout << T << endl;
            UnclosedBlock U(tam->level);
            cout << U.what();
          } else
            cout << T;
        } else {
          if (T[0] >= 48 && T[0] <= 57)
            cout << T << endl;
          else {
            cout << T;
            break;
          }
        }
      } else if (s == "PRINT")
        checkPRINT(list, S, line, count_line);
      else if (s == "RPRINT")
        checkRPRINT(list, S, line, count_line);
    }
    while (list.head != nullptr) {
      Node *tmp = list.head;
      list.head = list.head->next;
      delete tmp;
      tmp = nullptr;
      list.size = 0;
    }
  }
  new_file.close();
}

SymbolTable::List::~List() {
  while (this->head != nullptr) {
    Node *tmp = this->head;
    this->head = this->head->next;
    delete tmp;
    tmp = nullptr;
  }
  this->size = 0;
}
