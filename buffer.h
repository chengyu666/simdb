#include "global.h"
#include "dataStorage.h"

#ifndef BUFFER_H
#define BUFFER_H
class BMgr
{
public:
    BMgr(DSMgr *dsmgr);
    // Interface functions
    int FixPage(int page_id, int prot);
    // void NewPage FixNewPage();
    // int UnfixPage(int page_id);
    int NumFreeFrames();

    // Internal Functions
    //int SelectVictim();
    //int Hash(int page_id);
    //void RemoveBCB(BCB *ptr, int page_id);
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
};
#endif