#include "SymbolTable.h"

// ================== Symbol Table ===================
SymbolTable::SymbolTable(){
    this->curList = &global_list;
}
SymbolTable::~SymbolTable(){}
void SymbolTable::run(string filename) {
    ifstream ifs(filename);
    string line;
    if(ifs.is_open()){
        while (getline(ifs, line))
        {
            pair<string, int> command = process(line);
            if(command.first == "INSERT")
                this->insert(line);
            else if(command.first == "ASSIGN"){
                this->assign(line, command.second);
            }
            if(!ifs.eof())
                cout << endl;
        }
    }
    ifs.close();
    
}

pair<string, int> SymbolTable::process(string line){
    static const regex valid_insert("^INSERT[ ][a-z][a-zA-Z0-9_]*[ ](?:number|string)$");
    static const regex valid_assign("^ASSIGN[ ][a-z][a-zA-Z0-9_]*[ ](?:[a-z][a-zA-Z0-9_]*|\\d+|'[\\dA-Za-z\\s]+')$");
    //static const regex valid_block
    //static const regex valid_print
    //static const regex valid_lookup
    //static const regex valid_rprint
    pair<string, int> command;
    if(regex_match(line, valid_insert) == 1){ //CHECKVALID INSERT
        //Handle exception before return command
        handle_exception_insert(line);
        command.first = "INSERT"; command.second = 1;
        return command;
    }

    else if (regex_match(line, valid_assign)){ // CHECK VALID ASSGIN
        int type = handle_exception_assign(line);
        command.first = "ASSIGN"; command.second = type;
        return command;
    } 

    //Do not match all
    else{
        InvalidInstruction invalid = InvalidInstruction(line);
        throw invalid;
    }
}

void SymbolTable::insert(string line){
    //cout << "Can insert return " << this->can_insert(line) << endl;
    int start = line.find(" "), i = 0;
    string name = line.substr(start + 1, line.find(" ", start + 1) - start - 1);
    string type = line.substr(line.find(" ", start + 1) + 1);
    Identifier a = Identifier(name, type, "");
    Idn_Node *node = new Idn_Node(a);
    this->curList->insert_to_list(node);
    cout << "success";
}
void SymbolTable::assign(string line, int type){
    //MAKE SURE THAT WE CAN ASSIGN VALUE.
    //IF REACH THIS FUNCTION THEN ALL IDENTIFIERS EXISTED.
    int start = line.find(" "), i = 0;
    string name = line.substr(start + 1, line.find(" ", start + 1) - start - 1);
    string value = line.substr(line.find(" ", start + 1) + 1);
    Idn_Node *name_node = nullptr;
    Idn_List *tmp1 = this->curList;

    while(tmp1){
        name_node = tmp1->getNode(name);
        if(name_node != nullptr) 
            break;
        else
            tmp1 = tmp1->parent;
    }
    if (type != 0){   
        name_node->data.value = value;
        cout << "success";
    }
    else{
        Idn_List* tmp2 = this->curList;
        Idn_Node *val_node = nullptr;
        while(tmp2){
            val_node = tmp2->getNode(value);
            if(val_node != nullptr) break;
            else tmp2 = tmp2->parent;
        }
        name_node->data.value = val_node->data.value;
        cout <<"success";
    }
    
}

int SymbolTable::handle_exception_assign(string line){
// Return a number corresponding to the type of value
// If return successfully, then we can assign the value for identifier.
// 0 - IDENTIFIER
// 1 - NUMBER
// 2 - STRING
    //SEARCH PATTERN
    static const regex valid_identifer("^[a-z][a-zA-Z0-9_]*$");
    static const regex valid_number("^\\d+$");
    static const regex valid_string("^'[\\dA-Za-z\\s]+'$");

    int start = line.find(" "), i = 0;
    string name = line.substr(start + 1, line.find(" ", start + 1) - start - 1);
    string value = line.substr(line.find(" ", start + 1) + 1);
    Idn_Node *name_node = nullptr;
    //VERIFY IDENTIFIER DECLARATION
    Idn_List *tmp_list = this->curList;

    while(tmp_list){
        name_node = tmp_list->getNode(name);
        if(name_node != nullptr)
            break;
        else
            tmp_list = tmp_list->parent;
    }
    //IDENTIFIER HAD NOT BEEN DECLARED
    if(name_node == nullptr){
        throw Undeclared(line);
    }

    //IDENTIFIER DECLARED
    else{
        //VERIFY ACCEPTABLE VALUE
        //VALUE IS IDENTIFIER -> CHECK DECLARATION
        if(regex_match(value, valid_identifer)){
            Idn_List *_tmp = this->curList;
            Idn_Node *val_node = nullptr;
            while(_tmp){
                val_node = _tmp->getNode(value);
                if(val_node != nullptr) break;
                else _tmp = _tmp->parent;
            }
            //VALUE_IDENTIFIER NOT DECLARED
            if(val_node == nullptr){
                throw Undeclared(line);
            }
            //VALUE IDENTIFER DECLARED
            else{
                if(val_node->data.type == name_node->data.type) //MATCH TYPE
                    return 0;
                else{ //NOT MATCH TYPE
                    throw TypeMismatch(line);
                }
            }
        }

        //IF FOUND, VALUE IS NUMBER 
        else if(regex_match(value, valid_number)){ 
            if(name_node->data.type != "number"){
                //TYPE NOT MATCH
                throw TypeMismatch(line);
            }
            return 1; //Return when type of name is number and had been declared.
        }

        //IF FOUND, VALUE IS STRING
        else if (regex_match(value, valid_string)){
            
            if(name_node->data.type != "string"){
                //TYPE NOT MATCH
                throw TypeMismatch(line);
            }
            return 2; //Return when type of name is string and had been declared.
        }

        else{
            throw InvalidInstruction(line);
        }
    }
}
void SymbolTable::handle_exception_insert(string line){
    int start = line.find(" "), i = 0;
    string name = line.substr(start + 1, line.find(" ", start + 1) - start - 1);
    string type = line.substr(line.find(" ", start + 1) + 1);
    if(this->curList->find(name)){
        Redeclared a = Redeclared(line);
        throw a; //Found - Redeclared
    }
}
void SymbolTable::handle_exception_lookup(string line){

}

void SymbolTable::printList(){
    Idn_Node * tmp = this->curList->head;
    while(tmp){
        cout << tmp->data.name << " " <<tmp->data.value << endl;
        tmp = tmp->next;
    }
}

// ================== Identifier ===================
Identifier::Identifier(string name, string type, string value){
    this->name = name;
    this->type = type;
    this->value = value;
}
Identifier::Identifier(){

}
// ================== Identifier Node===================
Idn_Node::Idn_Node(Identifier data){
    this->data = data;
    this->next = nullptr;
    this->prev = nullptr;
}

Idn_Node::Idn_Node(){
    this->next = nullptr;
    this->prev = nullptr;
}


// ================== Identifier List===================
Idn_List::Idn_List(){
    this->head = nullptr;
    this->tail = nullptr;
    this->child = nullptr;
    this->parent = nullptr;
    this->size = 0;
}

void Idn_List::insert_to_list(Idn_Node *node){
    if(head == nullptr){
        head = node;
        tail = head;
    }
    else{
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    this->size++;
}
bool Idn_List::find(string name){
    Idn_Node *tmp = this->head;
    while(tmp){
        if(tmp->data.name == name)
            return true;
        else tmp = tmp->next;
    }
    return false;
}
//GET TYPE OF A IDENTIFIER

Idn_Node* Idn_List::getNode(string name){
    Idn_Node*tmp = this->head;
    while(tmp){
        if(tmp->data.name == name){
            return tmp;            
        }
        tmp = tmp->next;
    }
    return nullptr;
}