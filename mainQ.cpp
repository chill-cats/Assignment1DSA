#include "SymbolTable.h"
#include "SymbolTable.cpp"
using namespace std;

void test(string filename)
{
    SymbolTable *st = new SymbolTable();
    try
    {
        st->run(filename);
        //cout << "Track List size: " << st->trackList_print.size;
        // cout << st->numBlock << endl;
        //st->curList->printout();
    }
    catch (exception &e)
    {
        cout << e.what();
    }
    delete st;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;
    test(argv[1]);
    //delete[] argv;
    return 0;
}