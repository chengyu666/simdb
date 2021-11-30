#include "dataStorage.h"
using namespace std;

DSMgr::DSMgr()
{
    diskRead = 0;
    diskWrite = 0;
    numPages = 0;
    memset(pageused, 0, MAXPAGES * sizeof(bool));
    return;
}

int DSMgr::OpenFile(string filename)
{
    dbfile.open(filename, ios::in | ios::out);
    if (dbfile.is_open())
    {
        log(0, "data.dbf opened.");
        return 0;
    }
    else
    {
        log(1, "data.dbf open failed!");
        exit(-1);
    }
}

int DSMgr::CloseFile()
{
    dbfile.close();
    log(0, "data.dbf closed.");
    return 0;
}

//TODO 并没有真正读取文件
bFrame DSMgr::ReadPage(int page_id)
{
    diskRead++;
    log(0, "ReadPage: " + to_string(page_id));
    bFrame f;
    dbfile.seekg(page_id * PAGE_SIZE);
    dbfile.read((char *)f.data, FRAME_SIZE);
    if (DISK_DELAY > 0)
    {
        this_thread::sleep_for(chrono::milliseconds(DISK_DELAY));
    }
    return f;
}

//写入文件
int DSMgr::WritePage(int page_id, bFrame frm)
{
    diskWrite++;
    log(0, "WritePage: " + to_string(page_id));
    dbfile.seekp(page_id * FRAME_SIZE);
    dbfile.write(frm.data, FRAME_SIZE);
    if (DISK_DELAY > 0)
    {
        this_thread::sleep_for(chrono::milliseconds(DISK_DELAY));
    }
    return 0; //ok
}

void DSMgr::SetUse(int page_id, bool use_bit)
{
    pageused[page_id] = use_bit;
}

bool DSMgr::GetUse(int page_id)
{
    return pageused[page_id];
}

void DSMgr::IncNumPages()
{
    numPages++;
}

int DSMgr::GetNumPages()
{
    return numPages;
}
