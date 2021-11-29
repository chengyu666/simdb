#include "global.h"
#include "dataStorage.h"

#ifndef BUFFER_H
#define BUFFER_H
class BMgr
{
public:
    BMgr(DSMgr *dsmgr_i);
    // Interface functions
    int FixPage(int page_id, int prot);
    // int FixNewPage();
    // int UnfixPage(int page_id);
    int NumFreeFrames();
    bool FrameCheck(int frame_id);
    // Internal Functions
    //int SelectVictim();
    int Hash(int page_id);
    void RemoveBCB(BCB *ptr, int frame_id);

    //void RemoveLRUEle(int frid);
    void SetDirty(int frame_id);
    void UnsetDirty(int frame_id); //nouse?
    void WriteDirtys();
    void PrintFrame(int frame_id);

private:
    // Hash Table
    //int ftop[BUF_SIZE];
    //BCB *ptof[BUF_SIZE];
    BCB BCBtable[BUF_SIZE]; //store BCBs
    bFrame mem[BUF_SIZE];   //store frames
    DSMgr *dsmgr;
    int hit_count;
    int miss_count;
};
#endif