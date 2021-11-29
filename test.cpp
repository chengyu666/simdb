#include "dataStorage.h"
using namespace std;

int main()
{
    log(0, "test");
    DSMgr *dsmgr = new DSMgr();
    dsmgr->OpenFile("data.dbf");

    bFrame f;
    strcpy(f.data, "123456789");
    int frame_id = 666;
    dsmgr->WritePage(frame_id, f);
    bFrame newf = dsmgr->ReadPage(frame_id);
    log(0, newf.data);
    dsmgr->CloseFile();
    return 0;
}