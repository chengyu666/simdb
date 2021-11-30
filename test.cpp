#include "dataStorage.h"
#include "buffer.h"
using namespace std;

int main()
{
    log(0, "test");
    DSMgr *dsmgr = new DSMgr();
    dsmgr->OpenFile("data.dbf");
    /* test datastorage
    bFrame f;
    strcpy(f.data, "123456789");
    int frame_id = 666;
    dsmgr->WritePage(frame_id, f);
    bFrame newf = dsmgr->ReadPage(frame_id);
    log(0, newf.data);
    */
    // test buffer
    BMgr *bmgr = new BMgr(dsmgr);
    log(0, "free frame:" + to_string(bmgr->NumFreeFrames()));
    //bmgr->PrintFrame(666);
    //bmgr->SetDirty(666);
    dsmgr->CloseFile();
    return 0;
}