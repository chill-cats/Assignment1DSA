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
            bool _end = ifs.eof();
            pair<string, int> command = process(line);
            if(command.first == "INSERT"){
                this->insert(line);
                if(!_end) cout << endl;
            }
            else if(command.first == "ASSIGN"){
                this->assign(line, command.second);
                if(!_end) cout << endl;
            }
            else if (command.first == "LOOKUP"){
                this->lookup(line);
                if(!ifs.eof()) cout << endl;   
            }
            else if (command.first =="BEGIN"){
                this->new_scope();
            }
            else if (command.first =="END"){
                this->end_scope();
            }
            else if (command.first == "PRINT"){
                if(this->trackList_print.head != NULL){
                    this->print();   
                    if(!ifs.eof()) cout << endl;
                }
            }
            else if (command.first == "RPRINT"){
                if(this->trackList_print.head != NULL){
                    this->rprint();   
                    if(!ifs.eof()) cout << endl;
                }
            }   
        }
        handle_end_file();
    }
    this->cleanup();
    ifs.close();   
}

void SymbolTable::cleanup(){
    Idn_List *ptr_list = &global_list;
    while(ptr_list->child){
        Idn_List *tmp1 = ptr_list->child;
        ptr_list->destroy_list();
        ptr_list = tmp1;
    }
    // Idn_List *tmp2 = &this->trackList_print;
    // tmp2->destroy_list();

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
    node->setLevel(this->curList->level); //Set level
    this->curList->insert_to_list(node);

    //Tracking for print and rprint
    //create a new node and copy information from soure node
    Idn_Node *tmp = trackList_print.getNode(name); //Node containing the identifier exist or not
    if(tmp == NULL){ //If not exist yet
        Idn_Node *track_node = new Idn_Node(node->data); //Create a new node with same name and type
        track_node->setLevel(this->curList->level); //Set level
        trackList_print.insert_to_list(track_node);
    }
    else{
        //Delete the older node
        if(tmp->next == NULL){
            tmp->prev->next = NULL;
            delete tmp;
        }
        else{
            if(tmp == this->trackList_print.head){
                this->trackList_print.head = tmp->next;
                delete tmp;
            }
            else{
                tmp->prev->next = tmp->next;
                delete tmp;
            }
        }
        this->trackList_print.size--;
        //Insert new node
        Idn_Node *track_node = new Idn_Node(node->data); //Create a new node
        track_node->setLevel(this->curList->level);
        this->trackList_print.insert_to_list(track_node);
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
        name_node = tmp->getNode(name);
        if(name_node != NULL){
            cout << tmp->level;
            break;
        }  
        else tmp = tmp->parent; 
    }
    if(name_node == NULL){
        throw Undeclared(line);
    } 
}
void SymbolTable::new_scope(){
    Idn_List *inner_scope = new Idn_List;
    this->curList->child = inner_scope;
    inner_scope->parent = this->curList;
    inner_scope->level = inner_scope->parent->level + 1;
    this->curList = this->curList->child;
}
void SymbolTable::end_scope(){
     int current_level = this->curList->level;
     if(current_level == 0){
        throw UnknownBlock();
     }
     else{
         Idn_List *tmp = this->curList;
         this->curList = this->curList->parent;
         tmp->destroy_list();
     }
}
void SymbolTable::print(){
    this->trackList_print.printForward();
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
        Redeclared a = Redeclared(line);
        throw a; //Found - Redeclared
    }
}
void SymbolTable::handle_end_file(){
    int current_level = this->curList->level;
    if(current_level != 0){
        throw UnclosedBlock(current_level);
    }
}
void SymbolTable::rprint()
{
    this->trackList_print.printBackward();
}

void SymbolTable::printList(){
    Idn_List *tmp = this->curList;
    while(tmp){
        tmp->printout();
        tmp = tmp->parent;
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
    this->next = NULL;
    this->prev = NULL;
    this->node_level = 0;
}

Idn_Node::Idn_Node(){
    this->next = NULL;
    this->prev = NULL;
    this->node_level = 0;
}

void Idn_Node::setLevel(int level){
    this->node_level = level;
}
// ================== Identifier List===================
Idn_List::Idn_List(){
    this->head = NULL;
    this->tail = NULL;
    this->child = NULL;
    this->parent = NULL;
    this->size = 0;
    this->level = 0;
}

void Idn_List::insert_to_list(Idn_Node *node){
    if(head == NULL){
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
    return NULL;
}

void Idn_List::destroy_list(){
    Idn_Node *ptr = new Idn_Node;
    ptr = this->head;
    while(ptr){
        Idn_Node*tmp = ptr->next;
        delete ptr;
        ptr = tmp;
    }
}

void Idn_List::printForward(){
    Idn_Node *tmp = this->head;
    if(tmp == NULL);
    else{
        for(int i =0; i < this->size; i++){
            if(i != this->size - 1){
                cout << tmp->data.name <<"//"<<tmp->node_level <<" ";
                tmp = tmp->next;
            }

            else{
                cout << tmp->data.name <<"//"<<tmp->node_level;
                tmp = tmp->next;
            }
        }   
    }
}

void Idn_List::printBackward(){
    Idn_Node *tmp = this->tail;
    if(tmp == NULL){}
    else{
        for(int i = 0; i < this->size; i++){
            if(i == this->size - 1){
                cout << tmp->data.name<<"//"<<tmp->node_level;
                tmp = tmp->prev;
            }
            else{
                cout << tmp->data.name<<"//"<<tmp->node_level << " ";
                tmp = tmp->prev;
            }
        }
    } 
}

void Idn_List::printout(){
    if(this->head == NULL);
    else{
        Idn_Node *tmp = this->head;
        while(tmp){
            cout << tmp->data.name<<" -level: "<<tmp->node_level<<" value: " << tmp->data.value << endl;
            tmp = tmp->next;
        }
    }
    cout << this->size << endl;
}
