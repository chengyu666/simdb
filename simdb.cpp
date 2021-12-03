#include "dataStorage.h"
#include "buffer.h"
#define TRUELLY_READ false
using namespace std;

void readPage(int page_id, BMgr *bmgr)
{
    BCB *bcb = bmgr->FixPage(page_id, 0);
    if (TRUELLY_READ)
    {
        char content[10];
        bmgr->GetFrameContent(bcb->frame_id, content);
        cout << "page id: " << page_id << " content: " << content << endl;
    }
}

int main()
{
    DSMgr *dsmgr = new DSMgr();
    dsmgr->OpenFile("data.dbf");
    BMgr *bmgr = new BMgr(dsmgr);
    readPage(1, bmgr);
    readPage(2, bmgr);
    readPage(3, bmgr);
    dsmgr->CloseFile();
    return 0;
}