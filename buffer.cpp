#include "buffer.h"

BMgr::BMgr(DSMgr *dsmgr_i)
{
    this->dsmgr = dsmgr;
    hit_count = 0;
    miss_count = 0;
    for (int i = 0; i < BUF_SIZE; i++)
    {
        BCBtable[i].page_id = -1;
        BCBtable[i].frame_id = -1;
        BCBtable[i].dirty = false;
        BCBtable[i].latch = false;
        BCBtable[i].count = 0;
    }
}
int BMgr::FixPage(int page_id, int prot)
{
    int prob_frame_id = Hash(page_id);
    if (BCBtable[prob_frame_id].page_id == page_id)
    {
        //page is in buffer, hit
        hit_count++;
        return prob_frame_id;
    }
    else
    {
        //page is not in buffer, miss
        miss_count++;
        //TODO
    }
    return 0;
}

int BMgr::NumFreeFrames()
{
    int count = 0;
    //add count if frame is dirty
    for (int i = 0; i < BUF_SIZE; i++)
    {
        if (BCBtable[i].frame_id == -1)
        {
            count++;
        }
    }
    return count;
}

//return true if the frame is unused
bool BMgr::FrameCheck(int frame_id)
{
    if (BCBtable[frame_id].page_id == -1)
    {
        log(1, "try to R/W unused frame!");
        return true;
    }
    else
        return false;
}
//hash pageid to frameid
int BMgr::Hash(int page_id)
{
    return page_id % BUF_SIZE;
}

void BMgr::RemoveBCB(BCB *ptr, int frame_id)
{
    log(0, "remove bcb: " + to_string(frame_id));
    BCBtable[frame_id].page_id = -1;
    //TODO:remove bcb from lru chain
}

void BMgr::SetDirty(int frame_id)
{
    if (FrameCheck(frame_id))
        return;
    BCBtable[frame_id].dirty = true;
}

void BMgr::UnsetDirty(int frame_id)
{
    if (FrameCheck(frame_id))
        return;
    BCBtable[frame_id].dirty = false;
}
//write all dirty pages to disk
void BMgr::WriteDirtys()
{
    for (int i = 0; i < BUF_SIZE; i++)
    {
        if (FrameCheck(i))
            continue;
        if (BCBtable[i].dirty)
        {
            dsmgr->WritePage(BCBtable[i].page_id, mem[i]);
        }
    }
}

void BMgr::PrintFrame(int frame_id)
{
    cout << "----frame----\nframe_id: " << frame_id << endl;
    cout << "page_id: " << BCBtable[frame_id].page_id << endl;
    cout << "dirty: " << BCBtable[frame_id].dirty << endl;
    cout << "latch: " << BCBtable[frame_id].latch << endl;
    cout << "count: " << BCBtable[frame_id].count << endl;
    cout << "content: " << mem[frame_id].data << endl;
    cout << "-------------\n";
}