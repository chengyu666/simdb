#include "global.h"
#include "dataStorage.h"

#ifndef BUFFER_H
#define BUFFER_H
class BMgr
{
public:
    BMgr();
    // Interface functions
    BCB *FixPage(int page_id, int rw);
    // int FixNewPage();
    // int UnfixPage(int page_id);
    int NumFreeFrames();
    int GetHitCount();
    int GetMissCount();
    bool FrameCheck(BCB *bcb);
    // Internal Functions
    int Hash(int page_id);
    BCB *FindBCB_page(int page_id);
    BCB *FindBCB_frame(int frame_id);
    void ptof_add(BCB *bcb);
    void ptof_remove(BCB *bcb);
    void SetDirty(BCB *bcb);
    void UnsetDirty(BCB *bcb); //nouse?
    void LRU_WriteDirtys();
    void PrintFrame(BCB *bcb);
    //int LRU_findpage(int page_id);
    void LRU_fixpage(BCB *bcb, bool is_new);
    BCB *LRU_findandunfixvictim();
    BCB *findfreeframe();
    //void WriteFrameContent(int frame_id, char* content);
    void GetFrameContent(int frame_id, char *content);
    void SetFrameContent(int frame_id, char *content);
    DSMgr *dsmgr; //disk manager

private:
    // Hash Table
    int ftop[BUF_SIZE];       //frameid 2 pageid, -1 means empty
    hashitem *ptof[BUF_SIZE]; //buckets of hashitems
    BCB *head;                //LRU
    BCB *tail;                //MRU
    bFrame mem[BUF_SIZE];     //store frames
    int hit_count;
    int miss_count;
    int free_frame_num;
    string msg; //store debug message
};
#endif