#include "SymbolTable.h"
//NEWEST VERSION
// ================== Symbol Table ===================
SymbolTable::SymbolTable(){
    this->global_list = new Idn_List;
    this->curList = global_list;
}
SymbolTable::~SymbolTable(){
    this->cleanup();
}
void SymbolTable::run(string filename) {
    ifstream ifs(filename);
    string line;
    if(ifs.is_open()){
        while (getline(ifs, line))
        {
            bool _end = ifs.eof();
            pair<string, int> command = process(line);
            if(command.first == "INSERT"){
                this->insert(line);
                cout << endl;
            }
            else if(command.first == "ASSIGN"){
                this->assign(line, command.second);
                cout << endl;
            }
            else if (command.first == "LOOKUP"){
                this->lookup(line);
                cout << endl;
            }
            else if (command.first =="BEGIN")   this->new_scope();
            else if (command.first =="END")     this->end_scope();
            else if (command.first == "PRINT")  this->print();
            else if(command.first =="RPRINT")   this->rprint();
        }
        handle_end_file();
    }
    ifs.close();   
}

void SymbolTable::cleanup(){
    Idn_List *tmp = this->curList; //Start from the most inner scope
    while (tmp!= NULL)
    {
        Idn_List *del = tmp;
        tmp = tmp->parent;
        delete del;
    }

}
pair<string, int> SymbolTable::process(string line){
    static const regex valid_insert("^INSERT[ ][a-z][a-zA-Z0-9_]*[ ](?:number|string)$");
    static const regex valid_assign("^ASSIGN[ ][a-z][a-zA-Z0-9_]*[ ](?:[a-z][a-zA-Z0-9_]*|\\d+|'[\\dA-Za-z\\s]+')$");
    static const regex valid_begin("^BEGIN$");
    static const regex valid_end("^END$");
    static const regex valid_print("^PRINT$");
    static const regex valid_lookup("^LOOKUP[ ][a-z][a-zA-Z0-9_]*$");
    static const regex valid_rprint("^RPRINT$");
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
    else if (regex_match(line, valid_lookup)){
        command.first = "LOOKUP"; command.second = 1;
        return command;
    }
    else if (regex_match(line, valid_begin)){
        command.first = "BEGIN"; command.second = 1;
        return command;
    }
    else if(regex_match(line, valid_end)){
        command.first = "END"; command.second = 1;
        return command;
    }
    else if(regex_match(line, valid_print)){
        command.first = "PRINT"; command.second = 1;
        return command;
    }
    else if(regex_match(line, valid_rprint)){
        command.first = "RPRINT"; command.second = 1;
        return command;
    }
    //Do not match all
    else    throw InvalidInstruction(line);
}

void SymbolTable::insert(string line){
    int start = line.find(" "), i = 0;
    string name = line.substr(start + 1, line.find(" ", start + 1) - start - 1);
    string type = line.substr(line.find(" ", start + 1) + 1);

    Identifier a = Identifier(name, type, "");
    Idn_Node *main_node = new Idn_Node(a);
    main_node->setLevel(this->curList->level);
    this->curList->append(main_node);
    
    int current_level = this->curList->level;
    if(current_level != 0){
        Idn_List *tmp_track = this->curList->parent; //Finding the same name in the previous scope
        while (tmp_track)
        {
            Idn_Node *tmp_node = tmp_track->getNode(main_node->data.name);
            if(tmp_node != nullptr){
                tmp_node->enable = false;
            }
            tmp_track = tmp_track->parent;
        }
    }
    cout << "success";
}
void SymbolTable::assign(string line, int type){
    //MAKE SURE THAT WE CAN ASSIGN VALUE.
    //IF REACH THIS FUNCTION THEN ALL IDENTIFIERS EXISTED.
    int start = line.find(" "), i = 0;
    string name = line.substr(start + 1, line.find(" ", start + 1) - start - 1);
    string value = line.substr(line.find(" ", start + 1) + 1);
    Idn_Node *name_node = NULL;
    Idn_List *tmp1 = this->curList;

    while(tmp1){
        name_node = tmp1->getNode(name);
        if(name_node != NULL) 
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
        Idn_Node *val_node = NULL;
        while(tmp2){
            val_node = tmp2->getNode(value);
            if(val_node != NULL) break;
            else tmp2 = tmp2->parent;
        }
        name_node->data.value = val_node->data.value;
        cout <<"success";
    }
    
}
void SymbolTable::lookup(string line){
    int start = line.find(" "), i = 0;
    string name = line.substr(start + 1, line.find(" ", start + 1) - start - 1);
    Idn_List *tmp = this->curList;
    Idn_Node *name_node = NULL;
    while(tmp){
        name_node = tmp->getNode(name); //Check whether exist in current scope
        if(name_node != NULL){
            cout << tmp->level;
            break;
        }  
        else tmp = tmp->parent; //if not move to the outer scope
    }
    if(name_node == NULL){
        throw Undeclared(line); //If cannot find
    } 
}
void SymbolTable::new_scope(){
/*
The procedure will be:
1. Create a new list in heap.
2. Set the next (child) of current list is the new list.
3. Set the previous (parent) list of the new list is the current list.
4. Set the level of the new list is the level of current list + 1
5. Shift the current list to the new list.
*/
    Idn_List *new_list = new Idn_List;
    this->curList->child = new_list;
    new_list->parent = this->curList;
    new_list->level = this->curList->level + 1; //Set level for the inner scope
    this->curList = this->curList->child;
}
void SymbolTable::end_scope(){
    int current_level = this->curList->level;   
    //TRACKING FOR DISABLED AND ENABLED
    if(current_level != 0){
        Idn_List *tmp_track = this->curList->parent;
        while(tmp_track){ //Consider each outer list
            Idn_Node *track = this->curList->head;
            while(track){
                Idn_Node *tmp = tmp_track->getNode(track->data.name);
                if(tmp != nullptr){
                    tmp->enable = true;
                }
                track = track->next;
            }
            tmp_track = tmp_track->parent;
        }
    }
    if(current_level == 0){
        throw UnknownBlock();
    }
    else{
        //this->curList->parent->printout();
        Idn_List *tmp = this->curList;
        this->curList = this->curList->parent; //Shifting to the previous list.
        tmp->parent = nullptr; tmp->child = nullptr;
        delete tmp;
    }
    //Tracking to disable and enable 
}
void SymbolTable::print(){
    Idn_List *track_list = tracking();
    track_list->printBackward();
    delete track_list;
}
void SymbolTable::rprint(){
    Idn_List *track_list = tracking();
    track_list->printForward();
    delete track_list;
}
Idn_List* SymbolTable::tracking(){
    Idn_List * track_list = new Idn_List;
    Idn_List *tmp_track = this->curList;
    while(tmp_track){
        Idn_Node *tmp_tail = tmp_track->tail;
        while(tmp_tail){ //If tmp_head not null
            if(tmp_tail->enable == true){    //If ready to print
                if(track_list->find(tmp_tail->data.name) == false){
                    Idn_Node *print_node = new Idn_Node(tmp_tail->data);
                    print_node->setLevel(tmp_track->level);
                    track_list->append(print_node);
                }
            }
            tmp_tail = tmp_tail->prev;
        }
        tmp_track = tmp_track->parent;
    }
    return track_list;
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
    Idn_Node *name_node = NULL;
    //VERIFY IDENTIFIER DECLARATION
    Idn_List *tmp_list = this->curList;

    while(tmp_list){
        name_node = tmp_list->getNode(name);
        if(name_node != NULL)
            break;
        else
            tmp_list = tmp_list->parent;
    }
    //IDENTIFIER HAD NOT BEEN DECLARED
    if(name_node == NULL){
        throw Undeclared(line);
    }

    //IDENTIFIER DECLARED
    else{
        //VERIFY ACCEPTABLE VALUE
        //VALUE IS IDENTIFIER -> CHECK DECLARATION
        if(regex_match(value, valid_identifer)){
            Idn_List *_tmp = this->curList;
            Idn_Node *val_node = NULL;
            while(_tmp){
                val_node = _tmp->getNode(value);
                if(val_node != NULL) break;
                else _tmp = _tmp->parent;
            }
            //VALUE_IDENTIFIER NOT DECLARED
            if(val_node == NULL){
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
        throw Redeclared(line);
    }
}
void SymbolTable::handle_end_file(){
    int current_level = this->curList->level;
    if(current_level != 0){
        throw UnclosedBlock(current_level);
    }
}

// ================== Identifier ===================
Identifier::Identifier(string name, string type, string value){
    this->name = name;
    this->type = type;
    this->value = value;
}
Identifier::Identifier(){}
Identifier::~Identifier(){}

// ================== Identifier Node===================
Idn_Node::Idn_Node(Identifier data){
    this->data = data;
    this->next = NULL;
    this->prev = NULL;
    this->node_level = 0;
    this->enable = true;
}
Idn_Node::~Idn_Node(){}
Idn_Node::Idn_Node(){
    this->next = NULL;
    this->prev = NULL;
    this->node_level = 0;
    this->enable = true; //Default to be printed out.
}

void Idn_Node::setLevel(int level){
    this->node_level = level;
}
// ================== Identifier List===================
Idn_List::Idn_List(){
    this->head = NULL;
    this->tail = NULL;
    this->parent = NULL;
    this->child = NULL;
    this->size = 0;
    this->level = 0;
}
Idn_List::~Idn_List(){
    Idn_Node *tmp = this->head;
    if(tmp!=NULL){
        while(tmp){
            Idn_Node*del = tmp;
            tmp = tmp->next;
            delete del;
        }
    }
    this->parent = NULL;
    this->child = NULL;
}
void Idn_List::append(Idn_Node *node){
    if(this->head == NULL){
        this->head = node;
        this->tail = head;
    }
    else{
        this->tail->next = node;
        node->prev = tail;
        this->tail = node;
    }
    this->size++; 
}
bool Idn_List::find(string name){
    Idn_Node *tmp = this->head;
    if(tmp == NULL) return false;
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
    return NULL;
}
void Idn_List::printForward(){
    Idn_Node *tmp = this->head;
    if(tmp != nullptr){
        cout <<tmp->data.name<<"//"<<tmp->node_level;
        tmp = tmp->next;
        if(tmp == nullptr) cout << endl;
    }
    while(tmp){
        cout<<" "<<tmp->data.name<<"//"<<tmp->node_level;
        tmp = tmp->next;
        if(tmp == nullptr) cout << endl;
    }
}

void Idn_List::printBackward(){
    Idn_Node *tmp = this->tail;
    if(tmp != nullptr){
        cout <<tmp->data.name<<"//"<<tmp->node_level;
        tmp = tmp->prev;
        if(tmp == nullptr) cout << endl;
    }
    while(tmp){
        cout<<" "<<tmp->data.name<<"//"<<tmp->node_level;
        tmp = tmp->prev;
        if(tmp == nullptr) cout << endl;
    }
}
void Idn_List::destroy_node(){
    Idn_Node *tmp = this->head;
    if(tmp!=NULL){
        while(tmp){
            Idn_Node*del = tmp;
            tmp = tmp->next;
            delete del;
        }
    }
}
void Idn_List::printout(){
    if(this->head == NULL) cout <<"EMPTY" << endl; //Testing
    else{
        Idn_Node *tmp = this->head;
        while(tmp){
            cout << tmp->data.name<<" -level: "<<tmp->node_level<<" value: " << tmp->data.value <<"Enable: " << tmp->enable<< endl;
            tmp = tmp->next;
        }
    }
    cout << this->size << endl;
}