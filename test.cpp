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
    clock_t start, end;
    start = clock();
    BMgr *bmgr = new BMgr(dsmgr);
    log(0, "free frame:" + to_string(bmgr->NumFreeFrames()));
    bmgr->FixPage(666, 0);
    bmgr->FixPage(1, 1);
    bmgr->FixPage(666, 1);
    BCB *t = bmgr->FindBCB_page(1);
    bmgr->PrintFrame(t);
    log(0, "free frame:" + to_string(bmgr->NumFreeFrames()));
    //bmgr->PrintFrame(666);
    //bmgr->SetDirty(666);
    dsmgr->CloseFile();
    int i = 50000;
    while (i--)
        cout << "ohooooo" << endl;
    end = clock();
    log(0, "time:" + to_string((end - start) * 1.0 / CLOCKS_PER_SEC));
    return 0;
}