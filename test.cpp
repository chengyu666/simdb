#include "dataStorage.h"
using namespace std;


int main()
{
    log(0, "test");
    DSMgr *dsmgr = new DSMgr();
    dsmgr->OpenFile("data.dbf");
    dsmgr->CloseFile();
    
    return 0;
}