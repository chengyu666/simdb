#include "buffer.h"

BMgr::BMgr()
{
    this->dsmgr = new DSMgr();
    dsmgr->OpenFile("data.dbf");
    hit_count = 0;
    miss_count = 0;
    free_frame_num = BUF_SIZE;
    head = tail = nullptr;
    //init ftop with -1
    for (int i = 0; i < BUF_SIZE; i++)
    {
        ftop[i] = -1;
    }
}
BCB *BMgr::FixPage(int page_id, int rw)
{
    //rw: 0-read 1-write
    //check if page is in buffer, if not, add it to buffer
    BCB *result = FindBCB_page(page_id);
    if (result != nullptr)
    {
        //page is in buffer, hit
        hit_count++;
        msg = "hit: " + to_string(hit_count);
        msg += " all: " + to_string(hit_count + miss_count);
        log(2, msg);
        msg = "freeframe: " + to_string(free_frame_num);
        log(2, msg);
        //find its bcb
        BCB *bcb = FindBCB_page(page_id);
        //update its dirty
        if (rw == 1)
        {
            bcb->dirty = true;
        }
        //fix it
        LRU_fixpage(bcb, false);
        result = bcb;
    }
    else
    {
        //page is not in buffer, miss
        miss_count++;
        msg = "miss: " + to_string(miss_count);
        msg += " all: " + to_string(hit_count + miss_count);
        log(2, msg);
        msg = "freeframe: " + to_string(free_frame_num);
        log(2, msg);
        if (NumFreeFrames() == 0)
        {
            //buffer is full, find victim
            BCB *victim = LRU_findandunfixvictim();
            //PrintFrame(victim);
            //if dirty, write to disk
            if (victim->dirty) //TODO exist bug, not write
            {
                //dsmgr->WritePage(victim->page_id, mem[victim->frame_id]);
                dsmgr->diskWrite++;
                victim->dirty = false;
            }
            //remove it from ptof
            ptof_remove(victim);
            //change its page_id, the frame id is still the same
            victim->page_id = page_id;
            //update hash table
            ptof_add(victim);
            //update ftop
            ftop[victim->frame_id] = page_id;
            //fix it, it is new
            LRU_fixpage(victim, true);
            //read from disk
            dsmgr->diskRead++;
            result = victim;
        }
        else
        {
            //find free frame
            BCB *bcb = LRU_findfreeframe();
            //update its page_id
            bcb->page_id = page_id;
            //PrintFrame(bcb);
            //update hash table
            ptof_add(bcb);
            //update ftop
            ftop[bcb->frame_id] = page_id;
            //fix it
            LRU_fixpage(bcb, true);
            //read from disk
            dsmgr->diskRead++;
            result = bcb;
        }
    }
    if (result == nullptr)
    {
        log(1, "FixPage error");
        return nullptr;
    }
    if (rw == 1)
    {
        result->dirty = true;
    }
    return result;
}

int BMgr::NumFreeFrames()
{
    return free_frame_num;
}

//get hit count
int BMgr::GetHitCount()
{
    return hit_count;
}

//get miss count
int BMgr::GetMissCount()
{
    return miss_count;
}

//return true if the frame is unused
bool BMgr::FrameCheck(BCB *bcb)
{
    if (bcb->page_id == -1)
    {
        log(1, "try to R/W unused frame!");
        return true;
    }
    else
        return false;
}
//hash pageid to bucket id
int BMgr::Hash(int page_id)
{
    //bucket num is BUF_SIZE
    return page_id % BUF_SIZE;
}

BCB *BMgr::FindBCB_page(int page_id)
{
    int bucket_id = Hash(page_id);
    hashitem *p = ptof[bucket_id];
    if (p == nullptr)
    {
        log(2, "page_id: " + to_string(page_id) + " not in ptof");
    }
    while (p != nullptr)
    {
        if (p->page_id == page_id)
            //find the page, return BCB
            return p->bcb;
        p = p->next;
    }
    //not find the page, return nullptr
    return nullptr;
}

BCB *BMgr::FindBCB_frame(int frame_id)
{
    int page_id = ftop[frame_id];
    if (page_id == -1)
    {
        log(1, "try to find unexist frame!");
        return nullptr;
    }
    return FindBCB_page(page_id);
}

void BMgr::SetDirty(BCB *bcb)
{
    if (FrameCheck(bcb))
        return;
    bcb->dirty = true;
}

void BMgr::UnsetDirty(BCB *bcb)
{
    if (FrameCheck(bcb))
        return;
    bcb->dirty = false;
}
//write all dirty pages to disk
void BMgr::LRU_WriteDirtys()
{
    BCB *p = head;
    //look for dirty pages in LRU chain
    while (p != nullptr)
    {
        if (p->dirty)
        {
            //dsmgr->WritePage(p->page_id, mem[p->frame_id]);
            dsmgr->diskWrite++;
            p->dirty = false;
        }
        p = p->next;
    }
}

void BMgr::PrintFrame(BCB *bcb)
{
    cout << "----frame----\nframe_id: " << bcb->frame_id << endl;
    cout << "page_id: " << bcb->page_id << endl;
    cout << "dirty: " << bcb->dirty << endl;
    cout << "latch: " << bcb->latch << endl;
    cout << "count: " << bcb->count << endl;
    cout << "content: " << mem[bcb->frame_id].data << endl;
    cout << "-------------\n";
}
/*
int BMgr::LRU_findpage(int page_id)
{
    //if page is in buffer, return frame id
    //else return -1
    hashitem *p = ptof[Hash(page_id)];
    while (p != nullptr)
    {
        if (p->page_id == page_id)
            return p->bcb->frame_id;
        p = p->next;
    }
    log(0, "page not in buffer!");
    return -1;
}
*/
BCB *BMgr::LRU_findandunfixvictim()
{
    //makesure buffer is full before use this function
    //find the victim
    BCB *victim = head;
    //remove it from LRU chain
    head = head->next;
    victim->next = nullptr;
    return victim;
}

BCB *BMgr::LRU_findfreeframe()
{
    //makesure buffer is not full before use this function
    //TODO: find free frame fron ftop
    for (int i = 0; i < BUF_SIZE; i++)
    {
        //i is the frame id
        if (ftop[i] == -1)
        {
            free_frame_num--;
            BCB *bcb = new BCB;
            bcb->frame_id = i;
            bcb->page_id = -1;
            bcb->dirty = false;
            bcb->latch = false;
            bcb->count = 0;
            bcb->next = nullptr;
            return bcb;
        }
    }
    log(1, "buffer is full!");
    return nullptr;
}

void BMgr::ptof_add(BCB *bcb)
{
    if (bcb == nullptr)
    {
        log(1, "add nullptr to ptof");
        return;
    }
    int bucket_id = Hash(bcb->page_id);
    //check if already exist
    hashitem *p = ptof[bucket_id];
    while (p != nullptr)
    {
        if (p->page_id == bcb->page_id)
        {
            log(1, "hashitem already exist in ptof!");
            return;
        }
        p = p->next;
    }
    //create new hash item
    p = new hashitem;
    p->bcb = bcb;
    p->page_id = bcb->page_id;
    //add to head
    p->next = ptof[bucket_id];
    ptof[bucket_id] = p;
    log(2, "add to ptof: " + to_string(p->page_id));
}

void BMgr::ptof_remove(BCB *bcb)
{
    //remove BCB from ptof
    int bucket_id = Hash(bcb->page_id);
    hashitem *p = ptof[bucket_id];
    hashitem *q = nullptr;
    while (p != nullptr)
    {
        //find out bcb
        if (p->bcb == bcb)
        {
            //remove from ptof
            if (q == nullptr)
            {
                //p is head
                ptof[bucket_id] = p->next;
            }
            else
            {
                q->next = p->next;
            }
            delete p;
            return;
        }
        q = p;
        p = p->next;
    }
    log(1, "hashitem not in ptof!");
}

void BMgr::LRU_fixpage(BCB *bcb, bool is_new)
{
    //update LRU chain
    //makesure frame is used for this page
    int page_id = bcb->page_id;
    if (is_new)
    {
        //frame is new, add to chain
        if (head == nullptr)
        {
            //add as chian's first element
            log(2, "add first elem 2 LRU chain.");
            head = tail = bcb;
            return;
        }
        else
        {
            //add element to tail
            tail->next = bcb;
            tail = bcb;
            return;
        }
    }
    else
    {
        //frame is already in chain, update LRU chain
        if (head->page_id == page_id)
        {
            //page's frame is in head
            BCB *ptr = head;
            //remove head
            head = head->next;
            if (head == nullptr)
            {
                //only one element
                tail = nullptr;
            }
            //add it to tail
            tail->next = ptr;
            tail = ptr;
            ptr->next = nullptr;
            return;
        }
        else
        {
            //page's frame is not in head
            //find the page in chain
            BCB *ptr = head;
            while (ptr->next != nullptr)
            {
                if (ptr->next->page_id == page_id)
                {
                    //remove ptr->next
                    BCB *tmp = ptr->next;
                    ptr->next = ptr->next->next;
                    if (ptr->next == nullptr)
                    {
                        //only one element
                        tail = ptr;
                    }
                    //add it to tail
                    tail->next = tmp;
                    tail = tmp;
                    tmp->next = nullptr;
                    return;
                }
                ptr = ptr->next;
            }
        }
    }
}

void BMgr::GetFrameContent(int frame_id, char *content)
{
    //get bcb from frame id
    BCB *bcb = FindBCB_frame(frame_id);
    if (FrameCheck(bcb))
        return;
    strcpy(content, mem[frame_id].data);
}

void BMgr::SetFrameContent(int frame_id, char *content)
{
    //get bcb from frame id
    BCB *bcb = FindBCB_frame(frame_id);
    if (FrameCheck(bcb))
        return;
    strcpy(mem[frame_id].data, content);
}