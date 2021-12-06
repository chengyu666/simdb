#include "dataStorage.h"
#include "buffer.h"
#define TRUELLY_READ false
#define TRUELLY_WRITE false
using namespace std;
//DSMgr *dsmgr;
BMgr *bmgr;
string fileName = "data-5w-50w-zipf.txt";

void ReadPage(int page_id)
{
    BCB *bcb = bmgr->FixPage(page_id, 0);
    if (TRUELLY_READ)
    {
        //read content from memory
        char content[10];
        bmgr->GetFrameContent(bcb->frame_id, content);
        cout << "page id: " << page_id;
        cout << " content: " << content << endl;
    }
}

void WritePage(int page_id, char *content = nullptr)
{
    BCB *bcb = bmgr->FixPage(page_id, 1);
    if (!TRUELLY_WRITE)
        return;
    //write content to memory
    if (content != nullptr)
        bmgr->SetFrameContent(bcb->frame_id, content);
    else
    {
        char content[10] = "nothing..";
        bmgr->SetFrameContent(bcb->frame_id, content);
    }
}

int main()
{
    // = new DSMgr();
    //dsmgr->OpenFile("data.dbf");
    bmgr = new BMgr();
    ifstream fin(fileName);
    string line, rw, pageid;
    //record time cost
    clock_t start, end;
    start = clock();
    //read file into lines
    while (getline(fin, line))
    {
        rw = line.substr(0, 1);
        pageid = line.substr(2);
        //cout << rw << " " << pageid << endl;
        if (rw == "0")
        {
            ReadPage(stoi(pageid));
        }
        else if (rw == "1")
        {
            WritePage(stoi(pageid));
        }
        //break;
    }
    //write back all dirty pages
    bmgr->LRU_WriteDirtys();
    end = clock();

    int hit = bmgr->GetHitCount();
    int miss = bmgr->GetMissCount();
    int rcount = bmgr->dsmgr->GetDiskRead();
    int wcount = bmgr->dsmgr->GetDiskWrite();
    float time = (end - start) * 1.0 / CLOCKS_PER_SEC;

    log(0, "----- summary -----");
    log(0, "total requests: " + to_string(hit + miss));
    log(0, "time cost: " + to_string(time) + "s");
    log(0, "hit: " + to_string(hit) + ", miss: " + to_string(miss));
    log(0, "buffer hit rate: " + to_string(hit * 1.0 / (hit + miss)));
    log(0, "read count: " + to_string(rcount));
    log(0, "write count: " + to_string(wcount));

    fin.close();
    bmgr->dsmgr->CloseFile();
    return 0;
}