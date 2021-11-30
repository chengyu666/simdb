#include "buffer.h"

BMgr::BMgr(DSMgr *dsmgr_i)
{
    this->dsmgr = dsmgr;
    hit_count = 0;
    miss_count = 0;
    free_frame_num = BUF_SIZE;
    head = tail = nullptr;
}
BCB *BMgr::FixPage(int page_id, int prot)
{
    //check if page is in buffer, if not, add it to buffer
    //return frame id
    if (LRU_findpage(page_id))
    {
        //page is in buffer, hit
        hit_count++;
        BCB *bcb = FindBCB_page(page_id);
        //fix it
        LRU_fixpage(page_id, bcb);
        return bcb;
    }
    else
    {
        //page is not in buffer, miss
        miss_count++;
        if (NumFreeFrames() == BUF_SIZE)
        {
            //buffer is full, find victim
            BCB *victim = LRU_findvictim(page_id);
            //if dirty, write to disk
            if (victim->dirty)
            {
                dsmgr->WritePage(victim->page_id, mem[victim->frame_id]);
                victim->dirty = false;
            }
            //remove it from ptof
            ptof_remove(victim);
            //change its page_id
            victim->page_id = page_id;
            //add new bcb to ptof
            ptof_add(victim);
            //fix it
            LRU_fixpage(page_id, victim);
            return victim;
        }
        else
        {
            //TODO: find free frame
            BCB *bcb = LRU_findfreeframe();
            //fix it
            LRU_fixpage(page_id, bcb);
            return bcb;
        }
    }
    return nullptr;
}

int BMgr::NumFreeFrames()
{
    return free_frame_num;
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
//hash pageid to frameid
int BMgr::Hash(int page_id)
{
    //bucket num is BUF_SIZE
    return page_id % BUF_SIZE;
}

BCB *BMgr::FindBCB_page(int page_id)
{
    int bucket_id = Hash(page_id);
    hashitem *p = ptof[bucket_id];
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
    BCB *p = head;
    while (p != nullptr)
    {
        if (p->frame_id == frame_id)
            //find the frame, return BCB
            return p;
        p = p->next;
    }
    //not find the frame, return nullptr
    return nullptr;
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
void BMgr::WriteDirtys()
{
    BCB *p = head;
    //look for dirty pages in LRU chain
    while (p != nullptr)
    {
        if (p->dirty)
        {
            dsmgr->WritePage(p->page_id, mem[p->frame_id]);
            p->dirty = false;
        }
        p = p->next;
    }
}

void BMgr::PrintFrame(BCB *bcb)
{
    cout << "----frame----\nframe_id: " << frame_id << endl;
    cout << "page_id: " << bcb->page_id << endl;
    cout << "dirty: " << bcb->dirty << endl;
    cout << "latch: " << bcb->latch << endl;
    cout << "count: " << bcb->count << endl;
    cout << "content: " << mem[frame_id]->data << endl;
    cout << "-------------\n";
}
int BMgr::LRU_findpage(int page_id)
{
    //if page is in buffer, return frame id
    //else return -1
    int bucket_id = Hash(page_id);
    //TODO
    if (//TODO ptof[frame_id] != nullptr && //TODO ptof[frame_id]->page_id == page_id)
    {
        return frame_id;
    }
    else
    {
        log(0, "page not in buffer!");
        return -1;
    }
}

BCB *BMgr::LRU_findvictim()
{
    //makesure buffer is full before use this function
    return head;
}

void BMgr::ptof_add(BCB *bcb)
{
    //add BCB to ptof
    int bucket_id = Hash(bcb->page_id);
    hashitem *p = new hashitem;
    p->bcb = bcb;
    //add to head
    p->next = ptof[bucket_id];
    ptof[bucket_id] = p;
}

void BMgr::ptof_remove(BCB *bcb)
{
    //remove BCB from ptof
    int bucket_id = Hash(bcb->page_id);
    hashitem *p = ptof[bucket_id];
    hashitem *q = nullptr;
    while (p != nullptr)
    {
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
    log(1, "frame not in ptof!");
}

void BMgr::LRU_fixpage(int page_id, BCB *bcb)
{
    //update LRU chain
    //makesure frame is unused or use for this page
    if (bcb == nullptr)
    {
        //frame is unused
        BCB *new_bcb = new BCB;
        new_bcb->page_id = page_id;
        new_bcb->frame_id = frame_id;
        new_bcb->dirty = false;
        new_bcb->latch = false;
        new_bcb->count = 0;
        //update hash table
        ptof_add(new_bcb);
        ftop[frame_id] = page_id;
        if (head == nullptr)
        {
            //add first element
            log(0, "add first elem 2 LRU chain.");
            head = tail = new_bcb;
            return;
        }
        else
        {
            //add new element to tail
            tail->next = new_bcb;
            tail = new_bcb;
            return;
        }
    }
    else
    {
        //frame is used, update LRU chain
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
    if (FrameCheck(frame_id))
        return;
    strcpy(content, mem[frame_id].data);
}