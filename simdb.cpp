#include "dataStorage.h"
#include "buffer.h"
using namespace std;
DSMgr *dsmgr = new DSMgr();
BMgr *bmgr = new BMgr(dsmgr);

void readPage(int page_id)
{
    int frame_id = bmgr->FixPage(page_id);
    char content[10];
    bmgr->GetFrameContent(frame_id, content);
    cout<<"page id: "<<page_id<<" content: "<<content<<endl;
}

int main()
{
    dsmgr->OpenFile("data.dbf");
    readPage(1);
    dsmgr->CloseFile();
    return 0;
}