#include "SymbolTable.h"

string* Readfile(string filename);
string* Token(string k);
bool CheckIden(string iden, Symbol* store);
string CheckType(string value);
bool IdenValid(string word);
void Freeupspace(Symbol** k);

Symbol::Symbol(string iden, string tpe, int lev)
{
    this->identifier = iden;
    this->type = tpe;
    this -> level = lev;
}
void SymbolTable::run(string filename)
{
//initialize variable
    int  count1 = 0;
    int level = 0;
    int size[100];
    size[0] = 0;
    Symbol** danhsach = new Symbol * [100];
    for (int i = 0; i < 100; i++)
    {
        danhsach[i] = new Symbol[100];
    }
    string* k = Readfile(filename);

    //Read string
    while (k[count1] != "")
    {
        string* word = Token(k[count1]); // Tokenize
        {
            // Begin: increase level, make new size
            if (!strcmp(word[0].c_str(),"BEGIN"))
            {
                level += 1;
                size[level] = 0;
            }

                //End: decrease level, check for UnknownBlock
            else if (!strcmp(word[0].c_str(), "END"))
            {
                danhsach[level] = new Symbol[100];
                level -= 1;
                if (level < 0)
                {
                    Freeupspace(danhsach);
                    throw UnknownBlock();
                }
            }

                //Insert identifier
            else if (!strcmp(word[0].c_str(), "INSERT"))
            {
                if (!IdenValid(word[1])) // Check if identifier is valid
                {
                    Freeupspace(danhsach);
                    throw InvalidInstruction(k[count1]);
                }
                if (CheckIden(word[1], danhsach[level])) //check identifier in its level, true when there is no same iden
                {
                    cout << "success" << endl;
                    danhsach[level][size[level]] = Symbol(word[1], word[2], level);
                    size[level]++; //increase size
                }
                else //when there is iden
                {
                    Freeupspace(danhsach);
                    throw Redeclared(k[count1]);
                }
            }

                //Lookup
            else if (!strcmp(word[0].c_str(), "LOOKUP"))
            {
                int lev = level;
                bool NotFound = true; //false when found
                int Offi; //the level variable that appear at last
                while (lev >=  0 && NotFound) //iterate through every level
                {
                    int position = 0;
                    while (position < size[lev] && NotFound)
                    {
                        if (!strcmp(word[1].c_str() , danhsach[lev][position].getIdentifier().c_str())) //iterate through danhsach to find identifier
                        {
                            NotFound = false; //when there is iden, notfound become false.
                            Offi = lev;
                        }
                        position++;
                    }
                    lev -= 1;
                }
                if (!NotFound) //when found
                {
                    cout << Offi << "\n";
                }
                if (NotFound == true) //when not found
                {
                    Freeupspace(danhsach);
                    throw Undeclared(k[count1]);
                }
            }

                //Assign
            else if (!strcmp(word[0].c_str(), "ASSIGN"))
            {
                if (!IdenValid(word[1])) //check if the iden is valid
                {
                    Freeupspace(danhsach);
                    throw InvalidInstruction(k[count1]);
                }
                bool NotAssign = true; // when variable are assigned return false
                int lev = level;
                if (strcmp(CheckType(word[2]).c_str(), "Wrong") && strcmp(CheckType(word[2]).c_str(), "Variable"))// check if the value fit number, string or varible
                {
                    while (lev >= 0 && NotAssign) //Iterate through all level
                    {
                        if (!CheckIden(word[1], danhsach[lev]) == 1) //Check for identifier in its level
                        {
                            int position = 0;
                            while (position < size[lev]) //Iterate through danhsach
                            {
                                if (!strcmp(word[1].c_str() , danhsach[lev][position].getIdentifier().c_str()))
                                {
                                    if (!strcmp(CheckType(word[2]).c_str(), danhsach[lev][position].getType().c_str())) //check if it same type
                                    {
                                        cout << "success" << endl;
                                        NotAssign = false; //successfully assign
                                    }
                                    else //not the same type
                                    {
                                        Freeupspace(danhsach);
                                        throw TypeMismatch(k[count1]);
                                    }
                                }
                                position++;
                            }
                        }
                        lev -= 1;
                    }

                }
                else if (!strcmp(CheckType(word[2]).c_str(), "Variable"))
                {
                    int tang = level;
                    bool NotFound = true;
                    string a = "";
                    while (tang >= 0 && NotFound) //Iterate through all level
                    {
                        int position = 0;
                        while (position < size[tang] && NotFound) //Iterate through danhsach
                        {
                            if (!strcmp(word[2].c_str(), danhsach[tang][position].getIdentifier().c_str()))
                            {
                                a = danhsach[tang][position].getType();
                                NotFound = true;
                            }
                            position++;
                        }
                        tang--;
                    }

                    while (lev >= 0 && NotAssign) //Iterate through all level
                    {
                        if (!CheckIden(word[1], danhsach[lev]) == 1) //Check for identifier in its level
                        {
                            int position = 0;
                            while (position < size[lev]) //Iterate through danhsach
                            {
                                if (!strcmp(word[1].c_str(), danhsach[lev][position].getIdentifier().c_str()))
                                {
                                    if (!strcmp(a.c_str(), danhsach[lev][position].getType().c_str())) //check if it same type
                                    {
                                        cout << "success" << endl;
                                        NotAssign = false; //successfully assign
                                    }
                                    else //not the same type
                                    {
                                        Freeupspace(danhsach);
                                        throw TypeMismatch(k[count1]);
                                    }
                                }
                                position++;
                            }
                        }
                        lev -= 1;
                    }

                }
                else //type is not fit
                {
                    Freeupspace(danhsach);
                    throw InvalidInstruction(k[count1]);
                }
                if (NotAssign) //When there is no identifier fitted
                {
                    Freeupspace(danhsach);
                    throw Undeclared(k[count1]);
                }
            }

                //Print or RPrint
            else if (!strcmp(word[0].c_str(), "PRINT") || !strcmp(word[0].c_str(), "RPRINT"))
            {
                Symbol danhsachin[1000]; //create a danhsach to print
                int lev = level;
                int ele = 0;
                while (lev >= 0) //iterate through all levels
                {
                    for (int i = size[lev] - 1 ; i >= 0; i--)
                    {
                        if (CheckIden(danhsach[lev][i].getIdentifier(), danhsachin)) //if there is is identifier then do not add more
                        {
                            danhsachin[ele] = danhsach[lev][i];
                            ele++;
                        }
                        else continue;
                    }
                    lev--;
                }
                if (!strcmp(word[0].c_str(), "PRINT")) //print out danhsach in reverse order
                {
                    if (ele != 0)
                    {
                        for (int i = ele - 1; i > 0; i--)
                        {
                            cout << danhsachin[i].getIdentifier() << "//" << danhsachin[i].getLevel() << " ";
                        }
                        cout << danhsachin[0].getIdentifier() << "//" << danhsachin[0].getLevel();
                        cout << endl;
                    }
                }
                if (!strcmp(word[0].c_str(), "RPRINT")) //print out danhsach in right order
                {
                    if (ele != 0)
                    {
                        for (int i = 0; i < ele - 1; i++)
                        {
                            cout << danhsachin[i].getIdentifier() << "//" << danhsachin[i].getLevel() << " ";
                        }
                        cout << danhsachin[ele - 1].getIdentifier() << "//" << danhsachin[ele - 1].getLevel();
                        cout << endl;
                    }
                }
            }

                //no command, just gibberish
            else
            {
                Freeupspace(danhsach);
                throw InvalidInstruction(k[count1]);
            }
        }
        count1++;
    }
    //end of program, free all dynamic @rr@ys
    Freeupspace(danhsach);
    //print out UnclosedBlock when there is not enough end
    if (level > 0)
    {
        throw UnclosedBlock(level);
    }
}
//Use to read input
string* Readfile(string filename)
{
    ifstream infile(filename);
    string* line = new string[1000];
    string oneline;
    int i = 0;
    while (getline(infile, oneline))
    {
        line[i] = oneline;
        i++;
    }
    return line;
}

//Use to tokenize the input
string* Token(string k)
{
    static string a[10];
    string word;
    unsigned int n = 0;
    for (int i = 0; i < 3; i++)
    {
        while (n <= k.length() && k[n] != ' ')
        {
            word += k[n];
            n++;
        }
        a[i] = word;
        word = "";
        n++;
    }
    return a;
}

//Check if iden existed in the danhsach
bool CheckIden(string iden, Symbol* store)
{

    int i = 0;
    bool notlike = true;
    while (store[i].getIdentifier() != " " && notlike)
    {
        if (iden == store[i].getIdentifier()) notlike = false;
        i++;
    }
    return notlike;
}

//Check type of value
string CheckType(string word)
{
    regex number(R"(\d+)");
    regex string(R"('[a-zA-Z\d\s]*')");
    regex id(R"([a-z]\w*)");
    if (regex_match(word, number))
        return "number";
    if (regex_match(word, string))
        return "string";
    if (regex_match(word, id))
        return "Variable";
    return "Wrong";
}

//check if the iden is valid
bool IdenValid(string word)
{
    if (word[0] < 97 || word[0] > 122)
    {
        return false;
    }
    for (unsigned int j = 0; j < word.length(); j++)
    {
        if (!isalnum(word[j]) && word[j] != '_')
        {
            return false;
        }
    }
    return true;
}

//Function to free up the dynamic @rr@ys
inline void Freeupspace(Symbol** k)
{
    for (int i = 0; i < 100; i++)
    {
        delete[] k[i];
    }
    delete[] k;

}