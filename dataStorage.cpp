#include "dataStorage.h"
#include <string>
#include <fstream>
using namespace std;

fstream DSMgr::OpenFile(string filename)
{
    fstream dbfile;
    dbfile.open(filename, ios::in | ios::out);
    

    //TODO
}