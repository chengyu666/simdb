#include "buffer.h"

BMgr::BMgr(DSMgr *dsmgr)
{
    this->dsmgr = dsmgr;
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
    //TODO
    cout << "fix page.\n";
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

void BMgr::SetDirty(int frame_id)
{
    BCBtable[frame_id].dirty = true;
}

void BMgr::UnsetDirty(int frame_id)
{
    BCBtable[frame_id].dirty = false;
}
//write all dirty pages to disk
void BMgr::WriteDirtys()
{
    for (int i = 0; i < BUF_SIZE; i++)
    {
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